import pygame
from pygame.locals import *
from pygame import event
from common import *
import copy
from settings import *
import math

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

_ = 0

form_map = {
	'own': 'map1',
	't_size': 48,
	'minimap': [
		[1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
		[1, _, _, _, _, _, _, _, _, 1],
		[1, _, _, _, _, _, 1, 1, _, 1],
		[1, _, _, _, _, _, _, _, _, 1],
		[1, _, _, _, _, _, _, _, _, 1],
		[1, _, _, _, _, _, _, _, _, 1],
		[1, _, _, _, _, _, _, _, _, 1],
		[1, _, _, _, _, _, _, _, _, 1],
		[1, _, _, _, _, _, _, _, _, 1],
		[1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
	]
}

class TileMap:
	def __init__(self) -> None:
		self.tiles = {}
		self.tile_size = 1
		self.map_size = [WIDTH, HEIGHT]
		self.loaded = False

	def load(self, form_map):
		try:
			own_map, tile_size, minimap = form_map['own'], form_map['t_size'], form_map['minimap']
		except:
			if DEBUG_GAME: return print("Game error extract form map")
		self.tile_size = tile_size
		self.map_size = [len(minimap[0])*tile_size, len(minimap)*tile_size]
		for j, row in enumerate(minimap):
			for i, value in enumerate(row):
				key = str(i)+';'+str(j)
				self.tiles[key] = {
					'type': value,
					'own': own_map,
					'pos': [i, j]
				}
		self.loaded = True

	def tiles_around(self, pos):
		tiles = []
		for x in range(-1, 2):
			for y in range(-1, 2):
				check_pos = str(int(pos[0] / self.tile_size)+x+1) +';'+ str(int(pos[1] / self.tile_size)+y+1)
				if check_pos in self.tiles:
					tiles.append(self.tiles[check_pos])
		return tiles

	def draw(self, on_surf, offset=(0,0)):
		if not self.loaded: return
		for x in range( offset[0] // self.tile_size, (offset[0] + on_surf.get_width()) // self.tile_size + 1 ): 
			for y in range( offset[1] // self.tile_size, (offset[1] + on_surf.get_height()) // self.tile_size + 1 ):
				loc = str(x) +';'+ str(y)
				if loc in self.tiles and self.tiles[loc]['type'] == 1:
					tile = self.tiles[loc]
					pygame.draw.rect( 
						on_surf, 'darkgray', 
						( tile['pos'][0]*self.tile_size - offset[0], tile['pos'][1]*self.tile_size - offset[1], 
	   					self.tile_size, self.tile_size ), 2
					)

class Camera():

	def __init__(self, screen_size, map_size):
		self.real_scroll = [0, 0]
		self.scroll = [0, 0]
		self.map_size = map_size
		self.focus = [screen_size[0]/2, screen_size[1]/2]
		self.screen_size = screen_size
		self.delay = 10

	def update(self, focus):
		self.real_scroll[0] += (focus[0]-self.focus[0] - self.real_scroll[0]) / self.delay
		self.real_scroll[1] += (focus[1]-self.focus[1] - self.real_scroll[1]) / self.delay

		#Fix right side
		if self.real_scroll[0] + self.screen_size[0] < self.map_size[0]:
			self.scroll[0] = int(self.real_scroll[0])
		#Fix left side
		if self.real_scroll[0] <= 0: 
			self.scroll[0] = 0
		#Fix bottom side
		if self.real_scroll[1] + self.screen_size[1] < self.map_size[1]:
			self.scroll[1] = int(self.real_scroll[1])
		#Fix top side
		if self.real_scroll[1] <= 0:
			self.scroll[1] = 0

	def load(self, map_size): 
		self.map_size = map_size


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


class CHARACTER():

	def __init__(self, game) -> None:
		self.game = game
		self.event = event.Event(0)
		self.pid = "temp"

		self.__objs = {}

		self.__ip_ctrl = {
			'a':False, 'd': False, 'w': False, 's': False,
			'mouse_left': False, 'mouse_right': False,
		}
		self.mouse_pos = [0,0]

		self.__timing = {
			'kick_bullet': self.type_timming(1.0, cnt=1.0, is_loop=True),
			'kick_ball': self.type_timming(3.0, is_loop=True),
		}

		self.__objs[self.pid] = PhysicEntity([100, 100], 20, 3)

	def type_timming(self, dura=0, cnt=0, is_loop=False):
		return { 'dura': dura, 'cnt': cnt, 'loop': is_loop, 'avail': False }
	def me(self): return self.__objs[self.pid]

	def active_event(self, event):
		self.event = event
		m_pos = pygame.mouse.get_pos()
		offset = self.game.cam.scroll
		self.mouse_pos = [m_pos[0]+offset[0], m_pos[1]+offset[1]]

		if event.type == KEYDOWN:
			if event.key == K_a:
				self.__ip_ctrl['a'] = True
			if event.key == K_d:
				self.__ip_ctrl['d'] = True
			if event.key == K_w:
				self.__ip_ctrl['w'] = True
			if event.key == K_s:
				self.__ip_ctrl['s'] = True
		if event.type == KEYUP:
			if event.key == K_a:
				self.__ip_ctrl['a'] = False
			if event.key == K_d:
				self.__ip_ctrl['d'] = False
			if event.key == K_w:
				self.__ip_ctrl['w'] = False
			if event.key == K_s:
				self.__ip_ctrl['s'] = False
		if event.type == MOUSEBUTTONDOWN:
			if event.button == 1:
				self.__ip_ctrl['mouse_left'] = True
			if event.button == 3:
				self.__ip_ctrl['mouse_right'] = True
		if event.type == MOUSEBUTTONUP:
			if event.button == 1:
				self.__ip_ctrl['mouse_left'] = False
			if event.button == 3:
				self.__ip_ctrl['mouse_right'] = False

	def update(self, dtime):
		#Movement
		move_dir = [0, 0]
		if self.__ip_ctrl['a']: move_dir[0] = -1
		if self.__ip_ctrl['d']: move_dir[0] = 1
		if self.__ip_ctrl['w']: move_dir[1] = -1
		if self.__ip_ctrl['s']: move_dir[1] = 1
		
		self.me().movement(self.game.map_tiles, move_dir)
		if move_dir != [0, 0]: self.io_sput(CMD.CHAR_MOVE, move_dir, (self.me().pos))
		
		#Timming trigger
		self.update_timming(dtime)
		if self.timming_avail('kick_bullet'):
			# if self.event.type == MOUSEBUTTONUP and self.event.button == 1:
			if self.__ip_ctrl['mouse_left']:
				me_pos = self.me().center()
				b_dir = vec_dir_norm(me_pos, self.mouse_pos)
				self.game.entities.add( Bullet(me_pos, dir=b_dir), own=self.pid )
				self.io_put(CMD.CHAR_ACT, { 't_key': 'kick_bullet', 'dir':b_dir })
				print("Client add entity")

		if self.timming_avail('kick_ball'):
			print("Small ball")

	def update_timming(self, dtime):
		for t in self.__timing.values():
			if not t['loop'] and t['avail']: continue
			t['cnt'] += dtime
			if t['cnt'] >= t['dura']:
				t['cnt'] %= t['dura']
				t['avail'] = True
	def timming_avail(self, t_key):
		if t_key not in self.__timing: return False
		if self.__timing[t_key]['loop']:
			if self.__timing[t_key]['avail']:
				self.__timing[t_key]['avail'] = False
				return True
			else: return False
		return self.__timing[t_key]['avail']
	def timming_fresh(self, t_key, cnt_begin=0):
		if t_key not in self.__timing: return
		self.__timing[t_key]['cnt'] = cnt_begin
		self.__timing[t_key]['avail'] = False
	def get_vision(self, pid):
		return self.__objs[pid].get_vision()
	
	def movement(self, pid, dir):
		self.__objs[pid].movement(self.game.map_tiles, dir)
	
	def draw(self, on_surf, offset=(0,0)):
		for pid, obj in self.__objs.items():
			pygame.draw.rect(on_surf, 'red', (obj.pos[0]-offset[0], obj.pos[1]-offset[1], obj.size, obj.size), 4)
	

	####
	def io_sput(self, id, mes, state):
		out_mes = self.game.stpack_mes(id, mes)
		self.game.io_sput(out_mes, copy.copy((state)))
	def io_put(self, id, mes):
		self.game.io_put(self.game.out_mes(id, mes))

	def update_state_norm(self, pid, state):
		obj = self.__objs[pid]

		for id, s in state.items():
			
			if   id == 'pos': obj.pos = s
			elif id == 'speed': obj.speed = s
			elif id == 'size': obj.size = s

			else: pass
		
	def update_state_fast(self, pid, state):
		obj = self.__objs[pid]
		try:
			obj.pos = state['pos']
			obj.speed = state['speed']
		except:
			if DEBUG_GAME: print("Game error update state")

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
		if pid not in self.__objs: return None
		obj = self.__objs[pid]
		return {
			'pos': obj.pos,
			'speed': obj.speed,
		}
	def accept(self, pid, others):
		del self.__objs[self.pid]
		self.pid = pid
		for p, state in others.items():
			self.add(p, state)
	def add(self, pid, state):
		self.__objs[pid] = PhysicEntity()
		self.update_state_norm(pid, state)
	def remove(self, pid):
		try: del self.__objs[pid]
		except: 
			if DEBUG_GAME: print("Game error can not remove pid")

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
	
	def draw(self, on_surf, offset=(0,0)):
		pygame.draw.circle(on_surf, 'red', (self.pos[0]-offset[0], self.pos[1]-offset[1]), self.size, 3)

class Entities():

	def __init__(self, game) -> None:
		self.game = game
		self.__store = {}
		self.__count_idx = 0
		self.__idx_avail = []
		self.__must_remove = []

	def type_entity(self, type=None, own=None, obj=PhysicEntity() ):
		return { 'type': type, 'own': own, 'obj': obj }

	def add(self, entity, own=None):
		self.idx = self.__count_idx
		if len(self.__idx_avail) > 0:
			self.idx = self.__idx_avail.pop()
		else: self.__count_idx += 1
		self.__store[self.idx] = self.type_entity(entity.type, own, entity)
	def remove(self, e_idx):
		if e_idx not in self.__objs: return
		del self.__objs[e_idx]
		self.__must_remove.append(e_idx)

	def update(self, dtime):
		for e_idx, e in self.__store.items():
			e['obj'].update(dtime)
		
		while len(self.__must_remove) > 0:
			e_idx = self.__must_remove.pop()
			self.__idx_avail.append(e_idx)
			try: del self.__store[e_idx]
			except: continue

	def draw(self, on_surf, offset=(0,0)):
		for e_idx, e in self.__store.items():
			e['obj'].draw(on_surf, offset)