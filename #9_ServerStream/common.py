# https://docs.python.org/3/howto/sockets.html

import pygame
import pygame_gui
import math
import copy
import struct
import pickle
import json
import numpy as np

from pygame.locals import *
from pygame import event
from tie_mul.TieProcess import *
from tie_mul.TieThread import *

# HOST = '127.0.0.1'
# HOST = '192.168.137.61'
# PORT = 6234
BUFFER_RECV = 1024*1024

WIDTH  = 1000
HEIGHT = 600
SCALE  = 1
FPS = 60
CAM_WIDTH = 320
CAM_HEIGHT = 240

DEBUG_THREAD = False
FAKE_LAG = False

def enum(*sequential, **named):
	enums = dict(zip(sequential, range(len(sequential))), **named)
	return type('Enum', (), enums)

DT = enum (
	'STR',
	'SERIES',
	'CMD',
	'COUNT'
)

CMD = enum (
	'NONE',

	'CAM_CLI_BEG',
	'CAM_SER_PLAY',
	'CAM_SER_STOP',

	'AD_ACCEPT',
	'AD_REJECT',
	'AD_GET_LIST_CLIENT',
)

SERIES_ID = enum (
	'DATA',
	'CAM'
)

def tcp_unpack_desc(data):
	try:
		data_len, data_type = struct.unpack("<LL", data[:8])
		# print(f"Struct: {data_len}, {data_type}")
	except:
		print(f"Unpack desc: error")
		return False, None, None, None
	
	data = data[8:]

	#Not process data when
	if data_len > 1024*1024*10 or data_type >= DT.COUNT:
		return False, None, None, None
	
	return True, data_len, data_type, data

def tcp_unpack_data(data_type, data):
	if data_type == 0:
		try:
			data = data.decode()
			return True, data
		except:
			print(f"Unpack desc: decode failed")
			return False, data
	if data_type == 1:
		try:
			data = pickle.loads(data, fix_imports=True, encoding="bytes")
			return True, data
		except: 
			print(f"Unpack desc: pickle loads failed")
			return False, data
	if data_type == 2:
		try:
			data = struct.unpack("<L", data)[0]
			return True, data
		except: 
			print(f"Unpack desc: struct failed")
			return False, data
	pass

def tcp_pack(data_type, data):
	data_dump = data
	data_len = 0

	if   data_type == DT.STR:
		data_dump = data.encode()
		data_len = len(data)
		print(f"TCP-PACK: str len={data_len}, data={data_dump}")
	elif data_type == DT.SERIES:
		data_dump = pickle.dumps(data, 0)
		data_len = len(data_dump)
	else:
		data_dump = struct.pack("<L", data)
		data_len = 4
	data_desc = struct.pack("<LL", data_len, data_type)

	ret = data_desc+data_dump
	# print(f"Wrap pack({len(ret)}): {ret}")
	return ret



class AnimEntity():
	
	def __init__(self, sprite_sheet, src_rect, scale, amount_frame, dura, bg_color=(0,0,0,0)):

		self.sprite_sheet = sprite_sheet
		self.src_rect = src_rect
		self.amount_frame = amount_frame
		self.bg_color= bg_color

		self.frame = []
		self.width = src_rect[2]
		self.height = src_rect[3]
		self.scale = scale
		self.cur_idx_frame = 0
		self.dura = dura
		self.dura_timer = 0

		for i in range(amount_frame):
			image = pygame.Surface((src_rect[2], src_rect[3]))
			image.blit(
				self.sprite_sheet, (0, 0), 
				( src_rect[0] + i*src_rect[2], src_rect[1], src_rect[2], src_rect[3])
			)
			image = pygame.transform.scale(image, (scale*src_rect[2], scale*src_rect[3]))
			image.set_colorkey(bg_color)
			self.frame.append(image)

	def copy(self):
		return AnimEntity(self.sprite_sheet, self.src_rect, self.scale, self.amount_frame, self.dura, self.bg_color )

	def set_flip(self, flip_x, flip_y):
		for i in range(len(self.frame)):
			self.frame[i] = pygame.transform.flip( self.frame[i], flip_x, flip_y )

	def set_scale(self, scale):
		for i in range(len(self.frame)):
			self.frame[i] = pygame.transform.scale(self.frame[i], (scale*self.width, scale*self.height))

	def update(self, dtime):
		self.dura_timer += dtime
		if self.dura_timer >= self.dura:
			self.dura_timer %= self.dura
			self.cur_idx_frame += 1
			self.cur_idx_frame %= len(self.frame)

	def draw(self, on_surface, location, offset=(0,0),flip=False):
		on_surface.blit(
			pygame.transform.flip(self.frame[self.cur_idx_frame], flip, False), 
			(location[0] - offset[0], location[1] - offset[1])
		)
	
	def draw_with_scale(self, on_surface, location, offset=(0,0)):
		on_surface.blit(
			pygame.transform.scale(
				self.frame[self.cur_idx_frame], 
				( self.frame[self.cur_idx_frame].get_width()*self.scale, 
     			  self.frame[self.cur_idx_frame].get_height()*self.scale )
			),
			(location[0] - offset[0], location[1] - offset[1])
		)


class Assets():

	def __init__(self):
		self.default = {
			"no_signal": pygame.image.load("ui/no_signal_cam.jpg").convert_alpha(),
			"view_items": {
				"0": pygame.image.load("ui/no_signal_cam.jpg").convert_alpha(),
				"1": pygame.image.load("ui/no_signal_cam.jpg").convert_alpha(),
				"2": pygame.image.load("ui/no_signal_cam.jpg").convert_alpha(),
				"other": pygame.image.load("ui/no_signal_cam.jpg").convert_alpha(),
			}
		}