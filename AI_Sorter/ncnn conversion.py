from ultralytics import YOLO
model = YOLO("/home/dorin/SiemensTeamProject/AI-Market-Restocker/AI_Sorter/my_model.pt")
model.export(format="ncnn")