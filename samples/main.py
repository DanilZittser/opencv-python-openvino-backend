import argparse
from pathlib import Path
from statistics import mean
from typing import Dict, List

import cv2


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Run person detection inference')
    parser.add_argument('--xml', type=Path, required=True, help='Path to model *.xml file')
    parser.add_argument('--bin', type=Path, required=True, help='Path to model *.bin file')
    parser.add_argument('--video', type=Path, required=True, help='Path to video')

    args: Dict[str, Path] = vars(parser.parse_args())

    for filepath in args.values():
        assert filepath.exists()

    net = cv2.dnn.readNetFromModelOptimizer(str(args['xml']), str(args['bin']))
    net.setPreferableBackend(cv2.dnn.DNN_BACKEND_INFERENCE_ENGINE)

    capture = cv2.VideoCapture(str(args['video']))

    fps: List[float] = []

    while True:
        success, frame = capture.read()

        if not success:
            break

        net.setInput(cv2.dnn.blobFromImage(image=frame, size=(256, 256)))

        tic: int = cv2.getTickCount()
        outputs = net.forward()
        toc: int = cv2.getTickCount()
        fps.append(cv2.getTickFrequency() / (toc - tic))

        score: float = outputs[0, 0, 0, 2]
        left: int = int(outputs[0, 0, 0, 3] * frame.shape[1])
        top: int = int(outputs[0, 0, 0, 4] * frame.shape[0])
        right: int = int(outputs[0, 0, 0, 5] * frame.shape[1])
        bottom: int = int(outputs[0, 0, 0, 6] * frame.shape[0])

        cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)
        cv2.putText(frame, f'score = {score:.3f}', (left, top), 0, 1, (0, 255, 0), 2)
        cv2.putText(frame, f'fps = {mean(fps):.0f}', (0, 25), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        cv2.imshow('Press "Q" for exit', frame)

        key = cv2.waitKey(42) & 0xFF

        if key == ord('q'):
            break

    print(f'Throughput = {mean(fps):.2f} fps')
