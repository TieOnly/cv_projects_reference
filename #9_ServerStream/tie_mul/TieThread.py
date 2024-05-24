import threading

def thread_form(this, arg):
	if arg == None: this['target'](this)
	else: this['target'](this, arg)
	this['close'](id=this['id'])
	if this['debug']:
		print(f"Close task: {this['id']} - {this['name']}")

class ThreadRoster():
	
	def __init__(self, debug=True) -> None:
		self.thread_roster = {}
		self.count_id = 0
		self.__id_available__ = []
		self.debug = debug

	def __del__(self) -> None:
		self.close_all()
		if self.debug: print("Delete thread roster")

	def create(self, target, name, arg=None):
		id_avail = self.count_id
		if len(self.__id_available__):
			id_avail = self.__id_available__.pop()
		else: self.count_id += 1

		self.thread_roster[id_avail] = { 
			'id': id_avail, 'loop': True, 'name': name,
			'target': target, 'close': self.close, 'debug': self.debug
		}
		thread = threading.Thread(target=thread_form, args=(self.thread_roster[id_avail], arg))
		thread.start()
		if self.debug:
			print(f"Create thread: {id_avail}- {name}")

	def close(self, name=None, id=None):
		threads_del = []
		if id != None:
			if id in self.thread_roster:
				self.thread_roster[id]['loop'] = False
				self.__id_available__ .append(id)
				threads_del.append(id)
			
			for id in threads_del:
				del self.thread_roster[id]

			if self.debug and not len(threads_del): print(f"[ThreadRoster] -- not exist threading with id '{id}'")
			return

		if name != None:
			for thread in self.thread_roster.values():
				if thread['name'] == name:
					thread['loop'] = False
					self.__id_available__.append(thread['id'])
					threads_del.append(thread['id'])

			for id in threads_del:
				del self.thread_roster[id]

			if self.debug and not len(threads_del): print(f"[ThreadRoster] -- not exist threading with name '{name}'")
	
	def close_all(self):
		for thread in self.thread_roster.values():
			thread['loop'] = False
		self.count_id = 0
		self.__id_available__.clear()

	def thread_alive(self, name):
		for thread in self.thread_roster.values():
			if thread['name'] == name:
				return True
		return False