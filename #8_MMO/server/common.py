# https://docs.python.org/3/howto/sockets.html
import pygame
from pygame.locals import *
import math
import copy
import json

DEBUG_THREAD = False
DEBUG_EXTRACT = True
DEBUG_GAME = True

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
	try: return [payload['pid'], payload['form_mes'], payload['form_stream']]
	except:
		if DEBUG_EXTRACT: print("Error extract receive format")
		return [None, '', '']
def in_mes( payload):
	try: return [payload['id'], payload['mes']]
	except:
		if DEBUG_EXTRACT: print("Error extract mes format")
		return [None, None]
# def in_stream( payload):
# 	try: return [payload['stype'], payload['stream']]
# 	except:
# 		if DEBUG_EXTRACT: print("Error extract stream format")
# 		return [None, None]
def in_to_form( data):
	try: return json.loads(data)
	except: 
		if DEBUG_EXTRACT: print("Error extract data to form")
		return None
def out_form( form_id, payload):
	return { 'form': form_id, 'payload': payload }
def out_mes( pid, id, mes):
	payload = { 'pid': pid, 'id': id, 'mes': mes }
	return out_form(CMD.FORM_MES, payload)
def out_stream( stype, pid, stream):
	payload = { 'stype': stype, 'pid': pid, 'stream': stream }
	return out_form(CMD.FORM_STREAM, payload)
def out_to_data( out):
	try: return json.dumps(out)
	except:
		if DEBUG_EXTRACT: print("Error dumps outcome format to data")
		return None
	
def form_state( num, id, state):
	return { 'num': num, 'id': id, 'state': state }
def stpack_mes( id, mes):	
	return { 'id': id, 'mes': mes }
###

def vec_length(vec):
	return math.sqrt(vec[0]*vec[0] + vec[1]*vec[1])
def vec_normalize(vec):
	len = vec_length(vec)
	if len == 0: return [0, 0]
	return [vec[0] * (1/len), vec[1] * (1/len)]
def vec_dir_norm(vec_src, vec_dest):
	return vec_normalize((vec_dest[0]-vec_src[0], vec_dest[1]-vec_src[1]))


class PhysicEntity():

	def __init__(self, pos=(0,0), size=0, speed=0):
		self.pos = list(pos)
		self.size = size
		self.speed = speed

	def check_wall(self, tiles, tsize, pos_x, pos_y):
		left, right = int(pos_x/tsize), int((pos_x+self.size)/tsize)
		top, bottom = int(pos_y/tsize), int((pos_y+self.size)/tsize)

		for j in range(top, bottom+1):
			for i in range(left, right+1):
				loc = str(i)+';'+str(j)
				if loc in tiles and tiles[loc]['type'] == 1:
					return False, tiles[loc]['pos']
		return True, None

	def check_out_map(self, map_size):
		rect_map = pygame.Rect(0,0,map_size[0], map_size[1])
		return not rect_map.contains( self.pos[0], self.pos[1], self.size, self.size )

	def movement(self, map_obj, __dir=(0,0)):
		if abs(__dir[0]) > 1 or abs(__dir[1]) > 1: return
		dir = copy.copy(__dir)
		if dir[0] != 0:
			dir[0] *= self.speed
			no_collide, loc = self.check_wall(map_obj.tiles, map_obj.tile_size, self.pos[0]+dir[0], self.pos[1])
			if no_collide:
				self.pos[0] += dir[0]
			elif loc != None: 
				#make position nearest with tile
				if dir[0] < 0: self.pos[0] = (loc[0]+1)*map_obj.tile_size + 0.1
				else: self.pos[0] = loc[0]*map_obj.tile_size - self.size - 0.1

		
		if dir[1] != 0:
			dir[1] *= self.speed
			no_collide, loc = self.check_wall(map_obj.tiles, map_obj.tile_size, self.pos[0], self.pos[1]+dir[1])
			if no_collide:
				self.pos[1] += dir[1]
			elif loc != None: 
				#make position nearest with tile
				if dir[1] < 0: self.pos[1] = (loc[1]+1)*map_obj.tile_size + 0.1
				else: self.pos[1] = loc[1]*map_obj.tile_size - self.size - 0.1
		
		self.pos[0] = round(self.pos[0], 2)
		self.pos[1] = round(self.pos[1], 2)

	def update(self, dtime):
		pass

	def center(self):
		return [self.pos[0] + self.size/2, self.pos[1] + self.size/2]
	
	def rect(self):
		return pygame.Rect(self.pos[0], self.pos[1], self.size[0], self.size[1])
	
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