import json
# https://docs.python.org/3/howto/sockets.html

import pygame
from pygame.locals import *
from pygame import event
import math
import copy
import json
from tie_mul.TieProcess import *
from tie_mul.TieThread import *

DEBUG_THREAD = False
DEBUG_EXTRACT = False
DEBUG_GAME = True
FAKE_LAG = False

def enum(*sequential, **named):
    enums = dict(zip(sequential, range(len(sequential))), **named)
    return type('Enum', (), enums)

CMD = enum(

	'FORM_MES',
	'FORM_STREAM',
	'STYPE_MES',

	'SER_VALIDATE',
	'RST_SEQ_STATE',
	'STATE_OTHERS',

	'WAKE_UP',
	'MAP',
	'QUEUE',

	'CHAR_ACCEPT',
	'CHAR_ADD',
	'CHAR_REMOVE',
	'CHAR_MOVE',
	'CHAR_ACT_SKILL_PUT',
	'CHAR_ACT_SKILL_REMOVE',
    
	'ENTITY_REMOVE',

	'ERR_DATA_LEN',
	'ERR_PACK_LEN',

)

#form transmit
def in_form( payload):
	try: return [payload['form'], payload['payload']]
	except:
		if DEBUG_EXTRACT: print("Error extract receive format")
		return [None, None]
def in_mes( payload):
	try: return [payload['pid'], payload['id'], payload['mes']]
	except:
		if DEBUG_EXTRACT: print("Error extract mes format")
		return [None, None]
def in_state( payload):
	try: return [payload['id'], payload['qpos'], payload['state']]
	except:
		if DEBUG_EXTRACT: print("Error extract state format")
		return [None, None]
def in_stream( payload):
	try: return [payload['stype'], payload['pid'], payload['stream']]
	except:
		if DEBUG_EXTRACT: print("Error extract stream format")
		return [None, None, None]
def in_stpack_mes( payload):
	try: return [payload['id'], payload['mes']]
	except:
		if DEBUG_EXTRACT: print("Error extract stream pack mes format")
		return [None, None]
def in_to_form( data):
	try: return json.loads(data)
	except: 
		if DEBUG_EXTRACT: print("Error extract data to form")
		return None
def out_form( form_mes=None, form_stream=None):
	return { 'form_mes': form_mes, 'form_stream': form_stream }
def out_mes( id, mes):
	payload = { 'id': id, 'mes': mes }
	return out_form(form_mes=payload)
def out_stream( type, stream):
	payload = { 'stype': type, 'stream': stream }
	return out_form(form_stream=payload)
def out_to_data( out):
	try: return json.dumps(out)
	except:
		if DEBUG_EXTRACT: print("Error dumps outcome format to data")
		return None
	
def pack_mes(id, mes):
	return { 'id': id, 'mes': mes }
def pack_state(seq, state):
	return { 'num': seq, 'state': state }
def form_state(id, state):
	return { 'id': id, 'state': state }
###

def create_surface(sprite_sheet, src_rect, bg_color = (0, 0, 0)):
	image = pygame.Surface((src_rect[2], src_rect[3]))
	image.blit( sprite_sheet, (0, 0), src_rect )
	image.set_colorkey(bg_color)
	return image

def vec_length(vec):
	return math.sqrt(vec[0]*vec[0] + vec[1]*vec[1])
def vec_normalize(vec):
	len = vec_length(vec)
	if len == 0: return [0, 0]
	return [vec[0] * (1/len), vec[1] * (1/len)]
def vec_dir_norm(vec_src, vec_dest):
	return vec_normalize((vec_dest[0]-vec_src[0], vec_dest[1]-vec_src[1]))


DIR = enum(
	'LEFT',
	'RIGHT',
	'TOP',
	'BOTTOM'
)

class PhysicEntity():

	def __init__(self, pos=(0,0), size=0, speed=0):
		self.pos = list(pos)
		self.size = size
		self.speed = speed
		self.flip_x = False
		self.last_dir = DIR.RIGHT
		self.indented = 4

	def check_wall(self, tiles, tsize, pos_x, pos_y):
		left, right = int(pos_x/tsize), int((pos_x+self.size)/tsize)
		top, bottom = int(pos_y/tsize), int((pos_y+self.size)/tsize)

		for j in range(top, bottom+1):
			for i in range(left, right+1):
				loc = str(i)+';'+str(j)
				if loc in tiles and tiles[loc]['type'] == 1:
					return False, tiles[loc]['pos']
		return True, None

	def movement(self, map, __dir=(0,0)):
		if abs(__dir[0]) > 1 or abs(__dir[1]) > 1: return
		dir = copy.copy(__dir)
		if dir[0] != 0:
			dir[0] *= self.speed
			no_collide, loc = self.check_wall(map.tiles, map.tile_size, self.pos[0]+dir[0], self.pos[1])
			if no_collide:
				self.pos[0] += dir[0]
			elif loc != None: 
				#make position nearest with tile, update last direction
				if dir[0] < 0: 
					self.pos[0] = (loc[0]+1)*map.tile_size + 0.1
					self.last_dir = DIR.LEFT
				else: 
					self.pos[0] = loc[0]*map.tile_size - self.size - 0.1
					self.last_dir = DIR.RIGHT

		if dir[1] != 0:
			dir[1] *= self.speed
			no_collide, loc = self.check_wall(map.tiles, map.tile_size, self.pos[0], self.pos[1]+dir[1])
			if no_collide:
				self.pos[1] += dir[1]
			elif loc != None: 
				#make position nearest with tile, update last direction
				if dir[1] < 0: 
					self.pos[1] = (loc[1]+1)*map.tile_size + 0.1
					self.last_dir = DIR.TOP
				else: 
					self.pos[1] = loc[1]*map.tile_size - self.size - 0.1
					self.last_dir = DIR.BOTTOM

		self.pos[0] = round(self.pos[0], 2)
		self.pos[1] = round(self.pos[1], 2)

	def get_vision(self):
		delta = [0,0]
		if   self.last_dir == DIR.LEFT:
			delta[0] = -20
		elif self.last_dir == DIR.RIGHT:
			delta[0] = 20
		elif self.last_dir == DIR.TOP:
			delta[1] = -20
		else: delta[1] = 20
		return ( self.rect().centerx+delta[0], self.rect().centery+delta[1] )

	def update(self, dtime):
		pass
	def draw(self, on_surf, offet=(0,0)):
		pass

	def center(self):
		return [self.pos[0] + self.size/2, self.pos[1] + self.size/2]
	def rect(self):
		return pygame.Rect(self.pos[0], self.pos[1], self.size, self.size)
	
	def rect_offset(self, offet):
		return pygame.Rect(self.pos[0]-offet[0], self.pos[1]-offet[1]+self.indented, self.size[0], self.size[1])


data_player_tim = {
	'bullet': { 'dura': 0.3, 'cnt': 0.0, 'loop': False, 'avail': False },
	'a_bullet': { 'dura': 1.0, 'cnt': 0.0, 'loop': False, 'avail': False },
	'b_bullet': { 'dura': 3.0, 'cnt': 0.0, 'loop': False, 'avail': False },
	'c_bullet': { 'dura': 4.0, 'cnt': 0.0, 'loop': False, 'avail': False },
}

data_stuff_tim = {

}

class Tim():

	def __init__(self) -> None:
		self.__tim = {}
		self.__temp = {}
		pass

	
	def type_timming(self, dura=0, cnt=0, is_loop=False):
		return { 'dura': dura, 'cnt': cnt, 'loop': is_loop, 'avail': False }

	def get(self): return self.__tim

	def add_temp(self, temp): self.__temp = temp

	def add(self, t_name, t_ref):
		if t_ref not in self.__temp:
			if DEBUG_GAME: print("Tim {t_ref} cannot get from this template")
			return
		self.__tim[t_name] = copy.deepcopy(self.__temp[t_ref])

	def update(self, dtime):
		for t in self.__tim.values():
			if not t['loop'] and t['avail']: continue
			t['cnt'] += dtime
			if t['cnt'] >= t['dura']:
				t['cnt'] %= t['dura']
				t['avail'] = True
	def tim_avail(self, t_key):
		if t_key not in self.__tim: return False
		if self.__tim[t_key]['loop']:
			if self.__tim[t_key]['avail']:
				self.__tim[t_key]['avail'] = False
				return True
			else: return False
		return self.__tim[t_key]['avail']
	def tim_fresh(self, t_key, cnt_begin=0):
		if t_key not in self.__tim: return
		self.__tim[t_key]['cnt'] = cnt_begin
		self.__tim[t_key]['avail'] = False
