import cv2
from picamera2 import Picamera2
from ultralytics import YOLO
from bluepy import btle
from gpiozero import Button
import time

#Bluetooth
DEVICE_MAC = "F4:12:FA:70:AA:C5"
SERVICE_UUID = "180C"
CHAR_UUID = "2A56"
message = ""

# Camera setup
picam2 = Picamera2()
picam2.preview_configuration.main.size = (750, 750)
picam2.preview_configuration.main.format = "RGB888"
picam2.preview_configuration.align()
picam2.configure("preview")


print("Waiting to connect...")
time.sleep(3)

try:
    print("Connecting...")
    peripheral = btle.Peripheral(DEVICE_MAC)
    service = peripheral.getServiceByUUID(SERVICE_UUID)
    characteristic = service.getCharacteristics(CHAR_UUID)[0]
    print("Connected")
    picam2.start()
    picam2.capture_array()

except Exception as e:
    print(f"Bluetooth Connection Error: {e}")
    exit(1)


# Load YOLO model
model = YOLO("AI_Sorter/my_model_ncnn_model")

# Start
while True:

    frame = picam2.capture_array()
    
    results = model(frame)
    annotated_frame = results[0].plot()

    # Dettection check
    if results[0].boxes is not None and results[0].boxes.cls is not None:
        classes = results[0].boxes.cls
        labels = [model.names[int(cls)] for cls in classes]
        print(f"Dettected: {labels}")

        # Sending data via bl to ble
        try:
            message = labels[0]
            if message == "Red":
                message = 'R'
            elif message == "Green":
                message = 'G'
            elif message == "Yellow":
                message = 'Y'
            elif message == "Blue":
                message = 'B'
            elif message == "Orange":
                message = 'O'
            print(f"Sending message: {message}")
            characteristic.write(message.encode(), withResponse=True)
            time.sleep(2)
        except Exception as e:
            print(f"Error sending message: {e}")
    else:
        print("No Detection.")

    # FPS
    inference_time = results[0].speed['inference']
    fps = 1000 / inference_time
    text = f'FPS: {fps:.1f}'

    font = cv2.FONT_HERSHEY_SIMPLEX
    text_size = cv2.getTextSize(text, font, 1, 2)[0]
    text_x = annotated_frame.shape[1] - text_size[0] - 10
    text_y = text_size[1] + 10

    cv2.putText(annotated_frame, text, (text_x, text_y), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

    cv2.imshow("Camera", annotated_frame)

    # QUIT CAM - 'q'
    if cv2.waitKey(1) == ord("q"):
        break

# OBJ Search
cv2.destroyAllWindows()
