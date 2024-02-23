import pygame, sys
from pygame.locals import *
import json
from pbase64 import *

APP_W = 800 
APP_H = int(APP_W * 1 / 3)

def make_copy_surf(dest_surf, src_surf, contrs=120):
	for x in range(src_surf.get_width()):
		for y in range(src_surf.get_height()):
			col = [0, 0, 0]
			temp_col = src_surf.get_at((x, y))
			if temp_col[0] > contrs and temp_col[1] > contrs and temp_col[2] > contrs:
				col = [255, 255, 255]
			dest_surf.set_at((x, y), col)

def make_data_arr(surf):
	data_str = []

	for y in range(surf.get_height()):
		seg8 = 0x00
		i = 0x80
		for x in range(surf.get_width()):
			if surf.get_at((x, y))[0] == 0:
				seg8 |= i
			i >>= 1
			if i == 0x00:
				data_str.append(seg8)
				seg8 = 0x00
				i = 0x80
		if i != 0x80: data_str.append(seg8)

	f = open("arr.txt", 'w')
	f.write(str(tie_test_b64(bytes(data_str))))
	f.close()

def make_data_gif(surf, w, h):
	res = []
	for frame in range(int(surf.get_width()/w)):
		data_str = []
		for y in range(h):
			seg8 = 0x00
			i = 0x80
			for x in range((frame*w),((frame+1)*w)):
				if surf.get_at((x, y))[0] == 0:
					seg8 |= i
				i >>= 1
				if i == 0x00:
					data_str.append(seg8)
					seg8 = 0x00
					i = 0x80
			if i != 0x80: data_str.append(seg8)
		res.append(base64.b64encode(bytes(data_str)).decode())
	print(f"Done {surf}")
	return res

def make_write_json(file, v_data, v_w, v_h):
	f = open(file, 'w')
	json.dump({
		'f_amount': len(v_data),
		'f_width': v_w,
		'f_height': v_h,
		'data': v_data
	}, f)
	f.close()
	print("Done")

def make_list_data(path):
	f = open(path, 'r')
	video = json.load(f)
	print(video['f_amount'], video['f_width'], video['f_height'])
	print(len(video['data'][0]))

def make_surfs_scan():
	f = open("scan_gifs.json", 'r')
	gifs = json.load(f)
	surfs = []
	for gif in gifs['gif']:
		surfs.append(pygame.image.load(gif).convert_alpha())
	return surfs

class App:
	def __init__(self):
		pygame.init()
		pygame.display.set_caption("Make video 1 bit")
		
		self.clock = pygame.time.Clock()
		self.screen = pygame.display.set_mode((APP_W, APP_H), 0, 32)

		v_data = []
		for surf in make_surfs_scan():
			surf_1bit = pygame.Surface((surf.get_width(), surf.get_height()))
			make_copy_surf(surf_1bit, surf, 100)
			v_data += make_data_gif(surf_1bit, 84, 48)
		make_write_json('video.json', v_data, 84, 48)
		make_list_data("video.json")

	def run(self):
		while True:
			# self.screen.fill((100, 10, 20))

			for event in pygame.event.get():
				if event.type == QUIT:
					pygame.quit()
					sys.exit()

			# self.surf_demo.set_at((0,0), self.img_demo.get_at((0,0)))
			# print(self.surf_demo.get_at((0, 0)))

			# self.screen.blit(self.img_demo, (0, 0))
			# self.screen.blit(self.surf_demo, (0, self.img_demo.get_height()))

			pygame.display.update()
			self.clock.tick(24)
App().run()