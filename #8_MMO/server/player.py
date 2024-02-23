from common import *
import copy

form_info_phys_temp = {
	'pos': [100, 100], 'speed': 3, 'size': 20
}

form_info_skill_temp = {
	'skill_0': 'bullet',
	'skill_1': 'a_bullet',
}

form_info_temp = {
	'phys': { 'pos': [100, 100], 'speed': 3, 'size': 20 }, 
	'act': { 'hp': 0, 'armor': 0, 'power': 0 }, 
	'skill': form_info_skill_temp
}

data_all_skill = {
	'bullet': 	{ 'retime': 0.3, 'power': 10.0, 'size': 6, 'speed': 400, 'surf': 'red' },
	'a_bullet': { 'retime': 1.0, 'power': 20.0, 'size': 12, 'speed': 200, 'surf': 'yellow' },
	'b_bullet': { 'retime': 2.0, 'power': 20.0, 'size': [6,6], 'speed': 100, 'surf': 'green' },
	'c_bullet': { 'retime': 2.0, 'power': 20.0, 'size': [6,6], 'speed': 100, 'surf': 'gray' },
}

class SkillPlayer(PhysicEntity):

	def __init__(self, pos=(0,0), size=0, speed=0):
		super().__init__(pos, size, speed)
		self.info = {}
		self.name = "_"
		self.must_del = False

	def load(self, name):
		if name not in data_all_skill: 
			self.info = None
			return False
		self.info = copy.deepcopy(data_all_skill[name])
		self.name = name
		return True

	def update(self, dtime, game):
		pass


class Bullet(SkillPlayer):

	def __init__(self, name='bullet', pos=(0, 0), dir=(0,0)):
		super().__init__(pos)
		self.load(name)
		self.vel = [dir[0]*self.info['speed'], dir[1]*self.info['speed']]
	
	def update(self, dtime, game):
		self.pos[0] += self.vel[0] * dtime
		self.pos[1] += self.vel[1] * dtime
		if self.check_out_map(game.map_tiles.map_size):
			self.must_del = True


class ActPlayer():

	def __init__(self) -> None:
		self.info = { 'hp': 0, 'armor': 0, 'power': 0 }
		self.skills_cur = []
		self.skills_temp = {}
		self.skills_must_del = []
		self.state = {}
	
	def update(self, dtime, game):
		for idx, skill in enumerate(self.skills_cur):
			skill.update(dtime, game)
			if skill.must_del: self.skills_must_del.append(idx)


	#Put skill to world --- s_n: skill_idx, s_v: skill_varies
	def put_skill(self, s_idx, s_v):
		if s_idx not in self.skills_temp: 
			if DEBUG_GAME: print(f"Player cannot put skill {s_idx}")
			return False

		s_val = self.skills_temp[s_idx]

		if s_val == 'bullet' or s_val == 'a_bullet':
			
			self.skills_cur.append( Bullet(s_val, s_v['pos'], s_v['dir']) )
			if DEBUG_GAME: print(f"Player put skill {s_idx}")

		else: pass
		return True

class Player(PhysicEntity):

	def __init__(self, game):
		super().__init__()
		self.game = game
		self.act = ActPlayer()
		self.tim = Tim()
		self.tim.add_temp( data_player_tim )

		self.pid = None
		self.sock = None

	def load( self, form_info_phys=None, form_info_skill=None, form_info_act=None,
				pid=None, sock=None
		):
		#load info physic
		if form_info_phys != None:
			if 'pos' not in form_info_phys: return
			if 'size' not in form_info_phys: return
			if 'speed' not in form_info_phys: return
			self.pos = form_info_phys['pos']
			self.size = form_info_phys['size']
			self.speed = form_info_phys['speed']
		
		#load info skill, timming
		if form_info_skill != None:
			self.act.skills_temp = form_info_skill
			for s_idx, t_key in form_info_skill.items(): self.tim.add(s_idx, t_key)

		#load info act
		if form_info_act != None:
			if 'hp' not in form_info_act: return
			if 'armor' not in form_info_act: return
			if 'power' not in form_info_act: return
			self.act.info = form_info_act
		
		#Other info
		if pid != None: self.pid = pid
		if sock != None: self.sock = sock

	def get_info(self):
		return { 'info': { 'phys': {}, 'act': {}, 'skill': {} } }
	def get_tim(self):
		return self.tim.get()

	def update(self, dtime):
		
		self.tim.update(dtime)
		
		act = self.act
		act.update(dtime, self.game)
		while len(act.skills_must_del) > 0:
			idx = act.skills_must_del.pop()
			act.skills_cur.pop(idx)
			if DEBUG_GAME: print(f"Skill done - rest {len(act.skills_cur)}")

class PLAYERS():

	def __init__(self, game) -> None:
		self.game = game
		self.store = {}
	
	def type_players(self):
		return {
			'info': { 'map': {}, 'phys': {}, 'act': {}, 'skill': {} },
			'seq': { 'num': -1, 'state': None },
			'tim': {},
			'obj': Player(self.game)
		}

	#UPDATE
	def update_from_tick(self, dtime):
		for pid, player in self.store.items():
			player['obj'].update(dtime)

	def update_from_io_mes(self, connection, id, mes):

		sock, pid = connection
		p_obj = self.get_obj(pid)

		if   id == CMD.QUEUE:
			print(mes)
			self.game.io_putothers(pid, out_mes(pid, CMD.QUEUE, mes))

		elif id == CMD.RST_SEQ_STATE:
			self.get_seq(pid)['num'] = -1
			self.game.io_put(sock, out_mes(pid, id, 1))

		elif id == CMD.STATE_OTHERS:
			self.game.io_put(sock, out_mes(pid, id, self.get_field('info')))

		elif id == CMD.CHAR_ACT_SKILL_PUT:
			if 'sk_idx' not in mes: return
			if 'sk_varies' not in mes: return
			if 'pos' not in mes['sk_varies']: return
			if 'dir' not in mes['sk_varies']: return

			sk_idx, sk_varies = mes['sk_idx'], mes['sk_varies']
			if p_obj.tim.tim_avail(sk_idx):
				p_obj.tim.tim_fresh(sk_idx)
				put_ok = p_obj.act.put_skill( sk_idx, sk_varies )
				if put_ok: 
					self.game.io_putothers(pid, out_mes(pid, CMD.CHAR_ACT_SKILL_PUT, mes))

		else: pass

	def update_from_io_validate(self, pid, id, mes):

		state_validate = None
		p_obj = self.store[pid]['obj']

		if   id == CMD.CHAR_MOVE:
			if isinstance(mes, list) and len(mes) == 2:
				p_obj.movement(self.game.map_tiles, mes)
				state_validate = p_obj.pos
		
		elif id == CMD.QUEUE:
			print(mes)
			self.game.io_putothers(pid, out_mes(pid, CMD.QUEUE, mes))

		else: pass

		return state_validate
	###


	def exist(self, pid):
		return pid in self.store
	def get_obj(self, pid):
		if pid not in self.store: return None
		return self.store[pid]['obj']
	def get_info(self, pid):
		if pid not in self.store: return None
		return self.store[pid]['info']
	def get_seq(self, pid):
		if pid not in self.store: return None
		return self.store[pid]['seq']
	def get_field(self, field):
		if field not in ('info', 'obj', 'tim', 'seq'): return
		data = {}
		for pid, s in self.store.items(): data[pid] = s[field]
		return data

	def add(self, pid, sock, form_info):
		if 'phys' not in form_info: return
		if 'act' not in form_info: return
		if 'skill' not in form_info: return

		self.store[pid] = self.type_players()
		
		self.store[pid]['info'] = form_info
		self.store[pid]['obj'].load( form_info['phys'], form_info['skill'], form_info['act'] )
		self.store[pid]['obj'].load( pid=pid, sock=sock )
		self.store[pid]['tim'] = self.store[pid]['obj'].tim.get()
		if DEBUG_GAME: print(f"Player add {pid} - amount {len(self.store)}")
		

	def remove(self, pid):
		try: del self.store[pid]
		except:
			if DEBUG_GAME: print("Game error can not remove pid")


