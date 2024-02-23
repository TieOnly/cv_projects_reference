import cv2
import numpy as np
import base64
import json
import time
from TieThread import *

def get_saving_frames_durations(cap, saving_fps):
	s = []
	# get the clip duration by dividing number of frames by the number of frames per second
	clip_duration = cap.get(cv2.CAP_PROP_FRAME_COUNT) / cap.get(cv2.CAP_PROP_FPS)
	# use np.arange() to make floating-point steps
	for i in np.arange(0, clip_duration, 1 / saving_fps):
		s.append(i)
	return s, cap.get(cv2.CAP_PROP_FRAME_COUNT)

def make_data_img(surf, w, h):
	data_str = []
	for y in range(h):
		seg8 = 0x00
		i = 0x80
		for x in range(w):
			if surf[y, x] == 0:
				seg8 |= i
			i >>= 1
			if i == 0x00:
				data_str.append(seg8)
				seg8 = 0x00
				i = 0x80
		if i != 0x80: data_str.append(seg8)
	return base64.b64encode(bytes(data_str)).decode()

def make_write_json(file, v_data, v_w, v_h, v_dura):
	f = open(file, 'w')
	json.dump({
		'f_amount': len(v_data),
		'f_width': v_w,
		'f_height': v_h,
		'f_dura': v_dura,
		'data': v_data
	}, f)
	f.close()

def thread_count_processing(this, v_proc):
	while this['loop']:
		percent_proc = int(100*v_proc['count']/v_proc['total'])
		print(f"{percent_proc}%")
		time.sleep(1)

def main(video_file, fps_saving):
	cap = cv2.VideoCapture(video_file)
	fps = cap.get(cv2.CAP_PROP_FPS)
	fps_saving = max(fps_saving, 5)
	saving_frames_per_second = min(fps, fps_saving)
	print(f"Origin fps of video: {fps}")
	print(f"FPS for saving: {saving_frames_per_second}")

	saving_frames_durations, total_frame = get_saving_frames_durations(cap, saving_frames_per_second)
	count = 0
	
	threads = ThreadRoster(debug=False)
	v_proc = {'count': 0, 'total': total_frame}
	threads.create(thread_count_processing, 'count process', v_proc)
	v_data = []

	while True:
		is_read, frame = cap.read()
		if not is_read:
			print('100%')
			make_write_json('video.json', v_data, 84, 48, int(1000/saving_frames_per_second))
			break
		frame_duration = count / fps
		try:
			closest_duration = saving_frames_durations[0]
		except IndexError:
			print('100%')
			make_write_json('video.json', v_data, 84, 48, int(1000/saving_frames_per_second))
			break
		if frame_duration >= closest_duration:
			frame = cv2.resize(frame, (48, 84))
			frame = cv2.rotate(frame, cv2.ROTATE_90_COUNTERCLOCKWISE)
			frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
			(thresh, blackAndWhiteImage) = cv2.threshold(frame, 127, 255, cv2.THRESH_BINARY)
			v_data.append(make_data_img(blackAndWhiteImage, 84, 48))
			try:
				saving_frames_durations.pop(0)
			except IndexError:
				pass
		count += 1
		v_proc['count'] = count
		
	if threads.thread_alive('count process'):
		print("Done extract video.json")
		threads.__del__()

if __name__ == "__main__":
    import sys
    video_file = sys.argv[1]
    fps_saving = int(sys.argv[2])
    main(video_file, fps_saving)