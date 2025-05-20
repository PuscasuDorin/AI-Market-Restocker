import cv2
from picamera2 import Picamera2
from ultralytics import YOLO
from bluepy import btle
from gpiozero import Button
import time

# Configurare Bluetooth
DEVICE_MAC = "F4:12:FA:70:AA:C5"  # MAC-ul Arduino-ului tău
SERVICE_UUID = "180C"
CHAR_UUID = "2A56"
message = ""

# Așteaptă ca Arduino să înceapă publicarea BLE
print("Aștept 5 secunde ca Arduino să pornească BLE...")
time.sleep(5)

# Conectare Bluetooth
try:
    print("Conectare la Arduino...")
    peripheral = btle.Peripheral(DEVICE_MAC)
    service = peripheral.getServiceByUUID(SERVICE_UUID)
    characteristic = service.getCharacteristics(CHAR_UUID)[0]
    print("Conectat la Arduino!")
except Exception as e:
    print(f"Eroare de conexiune Bluetooth: {e}")
    exit(1)

# Setare camera
picam2 = Picamera2()
picam2.preview_configuration.main.size = (750, 750)
picam2.preview_configuration.main.format = "RGB888"
picam2.preview_configuration.align()
picam2.configure("preview")
picam2.start()

# Încarcă modelul YOLO
model = YOLO("AI_Sorter/my_model_ncnn_model")

# Bucla principală
while True:
    # Capture frame
    frame = picam2.capture_array()
    
    # Rulează YOLO pe frame
    results = model(frame)
    annotated_frame = results[0].plot()

    # Dacă sunt detecții
    if results[0].boxes is not None and results[0].boxes.cls is not None:
        classes = results[0].boxes.cls
        labels = [model.names[int(cls)] for cls in classes]
        print(f"Detectat: {labels}")

        # Trimite eticheta către Arduino
        try:
            message = labels[0]
            print(f"Trimit către Arduino: {message}")
            characteristic.write(message.encode(), withResponse=True)
        except Exception as e:
            print(f"Eroare la trimiterea către Arduino: {e}")
    else:
        print("Nicio detecție.")

    # FPS
    inference_time = results[0].speed['inference']
    fps = 1000 / inference_time
    text = f'FPS: {fps:.1f}'

    font = cv2.FONT_HERSHEY_SIMPLEX
    text_size = cv2.getTextSize(text, font, 1, 2)[0]
    text_x = annotated_frame.shape[1] - text_size[0] - 10
    text_y = text_size[1] + 10

    cv2.putText(annotated_frame, text, (text_x, text_y), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

    # Afișează frame-ul
    cv2.imshow("Camera", annotated_frame)

    # Ieșire dacă se apasă 'q'
    if cv2.waitKey(1) == ord("q"):
        break

# Curățare
cv2.destroyAllWindows()
