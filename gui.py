Qimport numpy as np
import cv2 as cv

def main():
    image_path = "ad.jpg"
    frame = cv.imread(image_path)

    if frame is None:
        print("Cannot open image")
        exit()

    # Add "NORTH" text in the top-left corner with a shadow effect
    cv.putText(frame, "NORTH", (20, 50),
                cv.FONT_HERSHEY_SIMPLEX, 1.5, (0, 0, 0), 5)  # Black shadow
    cv.putText(frame, "NORTH", (20, 50),
                cv.FONT_HERSHEY_SIMPLEX, 1.5, (255, 255, 255), 3)  # White text

    # Darkened rectangle for sensors
    x1, y1, x2, y2 = frame.shape[1] - 200, 10, frame.shape[1] - 10, 160
    overlay = frame.copy()
    cv.rectangle(overlay, (x1, y1), (x2, y2), (0, 0, 0), -1)
    alpha = 0.5
    cv.addWeighted(overlay, alpha, frame, 1 - alpha, 0, frame)

    # Sensors with depth
    sensors = ["TDS", "PH", "SM", "Temp"]
    depths = ["140", "3.6", "30m", "40g"]  # Example depths
    for i, (sensor, depth) in enumerate(zip(sensors, depths)):
        text = f"{sensor} ({depth})"
        cv.putText(frame, text, (x1 + 10, y1 + 30 + i * 30),
                    cv.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)

    # Add a sleek border and label "SENSORS"
    cv.rectangle(frame, (x1 - 5, y1 - 5), (x2 + 5, y2 + 30), (255, 255, 255), 2)
    cv.putText(frame, "Speed : 12 m/s", (x1, y2 + 25),
                cv.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)

    cv.imshow('frame', frame)
    
    cv.waitKey(0)  # Ожидание нажатия клавиши
    cv.destroyAllWindows()

if __name__ == "__main__":
    main()
