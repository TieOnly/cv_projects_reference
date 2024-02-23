from common import *

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
		self.map_size = 1
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

	def __init__(self, screen_size, map_size=None):
		self.real_scroll = [0, 0]
		self.scroll = [0, 0]
		self.map_size = map_size
		self.focus = [screen_size[0]/2, screen_size[1]/2]
		self.screen_size = screen_size
		if map_size == None: self.map_size = screen_size
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
