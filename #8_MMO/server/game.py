import pygame
from pygame.locals import *
from common import *
import copy
import math

def vec_length(vec):
	return math.sqrt(vec[0]*vec[0] + vec[1]*vec[1])
def vec_normalize(vec):
	len = vec_length(vec)
	if len == 0: return [0, 0]
	return [vec[0] * (1/len), vec[1] * (1/len)]
def vec_dir_norm(vec_src, vec_dest):
	return vec_normalize((vec_dest[0]-vec_src[0], vec_dest[1]-vec_src[1]))

PHYSIC_TILES = enum(
	'floor'
	'thick',
	'ceil',
)

_ = 0

form_map = {
	'own': 'map1',
	't_size': 48,
	'minimap': [
		[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
		[1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1],
		[1, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, 1],
		[1, _, 1, _, _, _, _, 1, 1, _, _, _, _, _, _, _, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, 1],
		[1, _, 1, _, _, _, _, 1, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1],
		[1, _, 1, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, _, _, 1],
		[1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, _, _, _, _, 1, 1, 1, _, _, 1],
		[1, _, _, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, 1, _, _, _, 1],
		[1, _, 1, _, _, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, 1, _, 1, 1, 1],
		[1, _, 1, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, 1, _, _, _, 1],
		[1, _, 1, _, _, _, _, 1, 1, _, _, _, _, _, _, _, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, 1],
		[1, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, _, 1],
		[1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1],
		[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
	]
}

class TileMap:
	def __init__(self) -> None:
		self.tiles = {}
		self.tile_size = None
		self.map_size = None

	def load(self, form_map):
		try:
			own_map, tile_size, minimap = form_map['own'], form_map['t_size'], form_map['minimap']
		except:
			if DEBUG_GAME: return print("Game error extract form map")
		self.tile_size = tile_size
		self.map_size = [len(minimap[0]), len(minimap)]
		for j, row in enumerate(minimap):
			for i, value in enumerate(row):
				key = str(i)+';'+str(j)
				self.tiles[key] = {
					'type': value,
					'own': own_map,
					'pos': [i, j]
				}

	def tiles_around(self, pos):
		tiles = []
		for x in range(-1, 2):
			for y in range(-1, 2):
				check_pos = str(int(pos[0] / self.tile_size)+x+1) +';'+ str(int(pos[1] / self.tile_size)+y+1)
				if check_pos in self.tiles:
					tiles.append(self.tiles[check_pos])
		return tiles

class PhysicEntity():

	def __init__(self, pos=(0,0), size=0, speed=0):
		self.pos = list(pos)
		self.size = size
		self.speed = speed
		self.flip_x = False
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

	def center(self):
		return [self.pos[0] + self.size/2, self.pos[1] + self.size/2]
	
	def rect(self):
		return pygame.Rect(self.pos[0], self.pos[1], self.size[0], self.size[1])
	
	def rect_offset(self, offet):
		return pygame.Rect(self.pos[0]-offet[0], self.pos[1]-offet[1]+self.indented, self.size[0], self.size[1])


class CHARACTER():

	def __init__(self, game) -> None:
		self.game = game
		self.__p_tim = game.store['p_tim']
		self.__objs = {}
		self.__objs['temp'] = PhysicEntity([100, 100], 20, 3)

	def type_timming(self, dura=0, cnt=0, is_loop=False):
		return { 'dura': dura, 'cnt': cnt, 'loop': is_loop, 'avail': False }
	def type_def_tim(self):
		return {
			'kick_bullet': self.type_timming(1.0),
			'kick_ball': self.type_timming(3.0, is_loop=True),
		}
	
	#Update game object
	def update(self, dtime):
		#Player timming update
		for pid, tim in self.__p_tim.items():

			self.update_tim(dtime, tim)
			
			if self.tim_avail(tim, 'kick_ball'):
				print('Kick ball')
				pass

	def update_tim(self, dtime, tim):
		for t in tim.values():
			if not t['loop'] and t['avail']: continue
			t['cnt'] += dtime
			if t['cnt'] >= t['dura']:
				t['cnt'] %= t['dura']
				t['avail'] = True
	def tim_avail(self, tim, t_key):
		if t_key not in tim: return False
		if tim[t_key]['loop']:
			if tim[t_key]['avail']:
				tim[t_key]['avail'] = False
				return True
			else: return False
		return tim[t_key]['avail']
	###

	#Update comming mes
	def update_comming_mes(self, pid, id, mes):
		
		if   id == CMD.CHAR_ACT:
			try: t_key = mes['t_key']
			except: return

			if self.tim_avail(self.__p_tim[pid], t_key):
				if t_key == 'kick_bullet':
					try: 
						if mes['dir'][0] > 1 or mes['dir'][1] > 1: return
					except: return
					b_pos = self.__objs[pid].center()
					self.game.entities.add( Bullet(b_pos, dir=mes['dir']) )
					print(f"{pid} - Act bullet")

		else: pass
	###
	
	def update_state_norm(self, pid, state):
		obj = self.__objs[pid]
		for id, s in state.items():
			if   id == 'pos': obj.pos = s
			elif id == 'speed': obj.speed = s
			elif id == 'size': obj.size = s
			else: pass

	def is_exist(self, pid):
		return pid in self.__objs
	def get(self, pid):
		if pid not in self.__objs: return None
		return self.__objs[pid]
	def get_state_norm(self, pid):
		if pid not in self.__objs: return None
		obj = self.__objs[pid]
		return {
			'pos': obj.pos,
			'speed': obj.speed,
			'size': obj.size,
		}
	def get_state_fast(self, pid):
		obj = self.__objs[pid]
		return {
			'pos': obj.pos,
			'speed': obj.speed,
		}
	def _add(self, pid, state):
		self.__objs[pid] = PhysicEntity()
		self.update_state_norm(pid, state)
	def _remove(self, pid):
		try: del self.__objs[pid]
		except:
			if DEBUG_GAME: print("Game error can not remove pid")

	def movement(self, pid, dir):
		self.__objs[pid].movement(self.game.map_tiles, dir)


class Bullet(PhysicEntity):

	def __init__(self, pos=(0, 0), size=10, speed=400, dir=(0,0)):
		super().__init__(pos, size, speed)
		self.type = 'bullet'
		self.vel = [dir[0]*speed, dir[1]*speed]
	
	def update(self, dtime):
		self.pos[0] += self.vel[0] * dtime
		self.pos[1] += self.vel[1] * dtime
	
	def check_out_map(self, map_size):
		rect_map = pygame.Rect(0,0,map_size[0], map_size[1])
		return not rect_map.contains( self.pos[0], self.pos[1], self.size, self.size )

class Entities():

	def __init__(self, game) -> None:
		self.game = game
		self.__objs = {}
		self.__count_idx = 0
		self.__idx_avail = []
		self.__must_remove = []

	def type_entity(self, type=None, own=None, obj=PhysicEntity()):
		return { 'type': type, 'own': own, 'obj': obj }
	
	def io_putall(self, id, mes):
		self.game.io_putall(self.game.out_mes(id, mes))

	def add(self, entity, own=None):
		self.idx = self.__count_idx
		if len(self.__idx_avail) > 0:
			self.idx = self.__idx_avail.pop()
		else: self.__count_idx += 1
		self.__objs[self.idx] = self.type_entity(entity.type, own, entity)
	
	def update(self, dtime):
		for e_idx, e in self.__objs.items():

			e['obj'].update(dtime)
			if e['type'] == 'bullet':
				if e['obj'].check_out_map(self.game.map_tiles.map_size):
					self.__must_remove.append(e_idx)
			else: pass
		
		while len(self.__must_remove) > 0:
			e_idx = self.__must_remove.pop()
			self.__idx_avail.append(e_idx)
			try: 
				del self.__objs[e_idx]
				self.io_putall(CMD.ENTITY_REMOVE, e_idx)
			except: continue

