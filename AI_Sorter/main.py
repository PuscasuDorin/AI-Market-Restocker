import cv2
from picamera2 import Picamera2
from ultralytics import YOLO
from bluepy import btle
from gpiozero import Button
import time

import json

#Bluetooth
DEVICE_MAC = "F4:12:FA:70:AA:C5"
SERVICE_UUID = "180C"
CHAR_UUID = "2A56"
message = ""
stock = [0, 0, 0, 0, 0]

#def update_item(stock, price, name):
#    data = {
#        "stock": int(stock),
#        "price": float(price),
#        "name": name
#    }

#    with open('stockdata.json', 'w') as f:
#        json.dump(data, f, indent=2)

 #   print("Item data updated!")

def update_stock(item_id, new_stock):
    # Citim datele din fisier
    with open('Online_Store/stockdata.json', 'r') as f:
        data = json.load(f)

    # Căutăm produsul după id și actualizăm stocul
    for item in data:
        if item['id'] == item_id:
            item['stock'] = int(new_stock)
            break
    else:
        print(f"Produs cu id {item_id} nu a fost găsit.")

    # Scriem datele actualizate înapoi în fisier
    with open('Online_Store/stockdata.json', 'w') as f:
        json.dump(data, f, indent=2)

    print(f"Stoc actualizat pentru produsul cu id {item_id} la {new_stock}.")

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
                stock[0] += 1
                if __name__ == "__main__":
                    update_stock(1,stock[0])
            elif message == "Green":
                message = 'G'
                stock[1] += 1
                if __name__ == "__main__":
                    update_stock(3,stock[1])
            elif message == "Yellow":
                message = 'Y'
                stock[2] += 1
                if __name__ == "__main__":
                    update_stock(5,stock[2])
            elif message == "Blue":
                message = 'B'
                stock[3] += 1
                if __name__ == "__main__":
                    update_stock(2,stock[3])
            elif message == "Orange":
                message = 'O'
                stock[4] += 1
                if __name__ == "__main__":
                    update_stock(4,stock[4])
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
