from common import *

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
		self.name = "_"
		self.info = {}
		self.must_del = False

	def load(self, name):
		if name not in data_all_skill: 
			self.info = None
			return False
		self.info = copy.deepcopy(data_all_skill[name])
		self.name = name
		return True

	def create(self, skill_varies):
		pass
	def update(self, dtime, game):
		pass
	def draw(self, on_surf, offet=(0,0)):
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
	
	def check_out_map(self, map_size):
		rect_map = pygame.Rect(0,0,map_size[0], map_size[1])
		return not rect_map.contains( self.pos[0], self.pos[1], self.size, self.size )
	
	def draw(self, on_surf, offset=(0,0)):
		pygame.draw.circle( on_surf, self.info['surf'], 
			(self.pos[0]-offset[0], self.pos[1]-offset[1]), self.info['size'], 3)

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
		while len(self.skills_must_del) > 0:
			idx = self.skills_must_del.pop()
			self.skills_cur.pop(idx)
			if DEBUG_GAME: print(f"Skill done - rest {len(self.skills_cur)}")
	
	def draw(self, on_surf, offet=(0,0)):
		for skill in self.skills_cur:
			skill.draw(on_surf, offet)

	#Put skill to world --- s_n: skill_idx, s_v: skill_varies
	def put_skill(self, s_idx, s_v):
		if s_idx not in self.skills_temp: 
			if DEBUG_GAME: print(f"Player cannot put skill {s_idx}")
			return

		s_val = self.skills_temp[s_idx]

		if s_val == 'bullet' or s_val == 'a_bullet':
			
			if 'pos' not in s_v: return
			if 'dir' not in s_v: return
			self.skills_cur.append( Bullet(s_val, s_v['pos'], s_v['dir']) )
			if DEBUG_GAME: print(f"Player put {s_idx} - {s_val}")

		else: pass

class Player(PhysicEntity):

	def __init__(self, game):
		super().__init__()
		self.game = game
		self.act = ActPlayer()
		self.tim = Tim()
		self.tim.add_temp( data_player_tim )
	
	def load(self, form_info_phys=None, form_info_skill=None, form_info_act=None):
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

	def get_info(self):
		return { 'info': { 'phys': {}, 'act': {}, 'skill': {} } }
	def get_tim(self):
		return self.tim.get()

	def update(self, dtime):
		self.tim.update(dtime)
		self.act.update(dtime, self.game)

	def draw(self, on_surf, offset=(0,0)):
		self.act.draw(on_surf, offset)
		pygame.draw.rect(on_surf, 'red', (self.pos[0]-offset[0], self.pos[1]-offset[1], self.size, self.size), 4)


class PLAYERS():

	def __init__(self, game) -> None:
		self.game = game
		self.store = {}
		self.event = event.Event(0)

		self.me_pid = game.gl_pid
		self.__mouse_pos = [0,0]
		self.__ip_ctrl = {
			'a':False, 'd':False, 'w':False, 's':False, 'e':False,
			'q':False,
			'mouse_left': False, 'mouse_right': False,
		}

		self.__seq_num = -1
		self.__q_state = Queue()
		self.__q_mes = Queue()

		self.add(self.me_pid, form_info_temp)

	def active_event(self, event):
		self.event = event
		m_pos = pygame.mouse.get_pos()
		offset = self.game.cam.scroll
		self.__mouse_pos = [m_pos[0]+offset[0], m_pos[1]+offset[1]]

		if event.type == KEYDOWN:
			self.__ip_ctrl[chr(event.key)] = True
		if event.type == KEYUP:
			self.__ip_ctrl[chr(event.key)] = False

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

	#UPDATE
	def update_from_tick(self, dtime):
		#Player object update
		for player in self.store.values(): player.update(dtime)
		#Me update
		me = self.me_obj()

		move_dir = [0, 0]
		if self.__ip_ctrl['a']: move_dir[0] = -1
		if self.__ip_ctrl['d']: move_dir[0] = 1
		if self.__ip_ctrl['w']: move_dir[1] = -1
		if self.__ip_ctrl['s']: move_dir[1] = 1

		if self.__ip_ctrl['q']:
			self.io_mput(CMD.QUEUE, "Hello")
		
		me.movement(self.game.map_tiles, move_dir)
		if move_dir != [0, 0]: 
			self.io_sput(CMD.CHAR_MOVE, move_dir, (me.pos))
		
		if self.__ip_ctrl['mouse_left']:
			
			if me.tim.tim_avail('skill_0'):
				me.tim.tim_fresh('skill_0')
				sk_dir = vec_dir_norm(me.center(), self.__mouse_pos)
				sk_varies = { 'pos': me.center(), 'dir': sk_dir }
				me.act.put_skill( 'skill_0', sk_varies )
				self.io_mput(CMD.CHAR_ACT_SKILL_PUT, { 'sk_idx': 'skill_0', 'sk_varies': sk_varies })
			
		if self.__ip_ctrl['e']:
			if me.tim.tim_avail('skill_1'):
				me.tim.tim_fresh('skill_1')
				sk_dir = vec_dir_norm(me.center(), self.__mouse_pos)
				sk_varies = { 'pos': me.center(), 'dir': sk_dir }
				me.act.put_skill( 'skill_1', sk_varies )
				self.io_mput(CMD.CHAR_ACT_SKILL_PUT, { 'sk_idx': 'skill_1', 'sk_varies': sk_varies })
		###

	def update_from_io_mes(self, pid, id, mes):
		
		#Server accept
		if   id == CMD.CHAR_ACCEPT:
			print(f"Accepted by server, pid: {mes['pid']}")
			self.accept(mes['pid'], mes['others'])
			self.game.map_tiles.load(mes['map'])
			self.game.cam.load(self.game.map_tiles.map_size)
		elif self.me_pid == 'temp': return
		###

		#Character
		if id == CMD.CHAR_ADD:
			print(f"Add player, pid: {mes['pid']}")
			self.add(mes['pid'], mes['state'])

		elif id == CMD.CHAR_REMOVE:
			print(f"Remove player, pid: {mes}")
			self.remove(mes)
		
		elif id == CMD.CHAR_ACT_SKILL_PUT:
			p_obj = self.get_obj(pid)
			if p_obj == None: return
			sk_idx, sk_varies = mes['sk_idx'], mes['sk_varies']
			p_obj.act.put_skill( sk_idx, sk_varies )
		###

		#Server validate
		elif id == CMD.SER_VALIDATE:

			ser_seq, ser_id, ser_state = mes['num'], mes['id'], mes['state']

			must_update_as_ser = True
			while not self.__q_state.empty():
				message, seq = self.__q_state.get(block=False)
				seq_num, seq_state = seq['num'], seq['state']
				print(f"({seq_num}-{ser_seq}) {seq_state} vs {ser_state}")
				if seq_num != ser_seq: continue

				must_update_as_ser = (seq_state != ser_state)
				break

			if must_update_as_ser:
				print('not same')
				self.__seq_num = ser_seq
				self.clear_expected_state()
				return self.update_as_ser(ser_id, ser_state)

			if self.__seq_num > 99 and self.__q_state.empty():
				self.__seq_num = -1
				self.io_mput(CMD.RST_SEQ_STATE, 1)

		elif id == CMD.RST_SEQ_STATE: pass

		elif id == CMD.STATE_OTHERS:
			try: 
				mes.pop(self.me_pid)
				mes[self.me_pid] = self.me_info()['phys']
				for pid, form in mes.items():
					self.get_obj(pid).load(form_info_phys=form)
			except:
				if DEBUG_GAME: print("Error change state others")
		###

		#Stuff
		elif id == CMD.QUEUE:
			print(f"From {pid}: {mes}")
		###

		#Error
		elif id == CMD.ERR_DATA_LEN:
			print(f"Error service {id}: data length")
			self.me_obj().load(form_info_phys=mes)
			self.clear_expected_state()

		elif id == CMD.ERR_PACK_LEN:
			print(f"Error service {id}: pack new len {mes['num_pack_new']}")
			self.clear_expected_state()
			for pid, form in mes['players'].items():
				self.get_obj(pid).load(form_info_phys=form)
		###
		
		else: pass

	def update_from_io_stream_mes(self, pid, stream):
		for pack in stream:
			id, mes = in_stpack_mes(pack)
			if id == None or mes == None: return

			if   id == CMD.CHAR_MOVE:
				self.get_obj(pid).movement(self.game.map_tiles, mes)

			else: pass

	def update_as_ser(self, id, state):

		if   id == CMD.CHAR_MOVE:
			self.me_obj().pos = state

		else: pass


	def draw(self, on_surf, offset=(0,0)):
		for p in self.store.values():
			p.draw(on_surf, offset)
	###

	def me_obj(self): return self.store[self.me_pid]
	def me_info(self): return self.store[self.me_pid].get_info()
	def me_tim(self): return self.store[self.me_pid].get_tim()
	def me_seq_num(self): return self.__seq_num

	def exist(self, pid):
		return pid in self.store
	def get_obj(self, pid):
		if pid not in self.store: return None
		return self.store[pid]
	def get_info(self, pid):
		if pid not in self.store: return None
		return self.store[pid].get_info()
	

	def io_mput(self, id, mes):
		if DEBUG_EXTRACT: print(f"mput {self.__q_mes.qsize()}")
		self.__q_mes.put(pack_mes(id, mes))
	def io_sput(self, id, mes, state):
		if DEBUG_EXTRACT: print(f"sput {self.__q_state.qsize()}")
		if self.__q_state.qsize() > 500: self.clear_expected_state()
		self.__seq_num += 1
		out_state = pack_state(self.__seq_num, state)
		self.__q_state.put([pack_mes(id, mes), out_state])
	def io_start(self):
		mes_pl, stream_pl = None, None
		if not self.__q_mes.empty():
			mes_pl = list(self.__q_mes.queue)
		if not self.__q_state.empty(): 
			stream_pl = list(self.__q_state.queue)
		if mes_pl == None and stream_pl == None: return
		self.game.io_put(out_form(form_mes=mes_pl, form_stream=stream_pl))
		while not self.__q_mes.empty(): self.__q_mes.get()
	def clear_expected_state(self):
		while not self.__q_state.empty(): self.__q_state.get()

	def accept(self, pid, others):
		del self.store[self.me_pid]
		self.game.gl_pid = pid
		self.me_pid = pid
		for p, info in others.items():
			self.add(p, info)

	def add(self, pid, form_info):
		if 'phys' not in form_info: return
		if 'act' not in form_info: return
		if 'skill' not in form_info: return

		self.store[pid] = Player(self.game)
		self.store[pid].load( form_info['phys'], form_info['skill'], form_info['act'] )

	def remove(self, pid):
		try: del self.store[pid]
		except:
			if DEBUG_GAME: print("Game error can not remove pid")