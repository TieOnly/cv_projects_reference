from common import *

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
		self.map_size = [len(minimap[0])*tile_size, len(minimap)*tile_size]
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