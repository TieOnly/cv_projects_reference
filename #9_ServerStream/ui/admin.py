import pygame.examples
from common import *

class UI_ADMIN_VAR():
	def __init__(self) -> None:
		self.func_sel_cam = None

class UI_ADMIN():

	def __init__(self, app) -> None:
		self.app = app
		self.assets = app.assets
		self.man = pygame_gui.UIManager((WIDTH, HEIGHT))
		# self.surf = {
		# 	'cam': {
		# 		1: {
		# 			'surf_no_signal': surf,
		# 			'surf': surf,
		# 			'offs': cam_offs
		# 		}
		# 	},
		# 	'view': {
		# 		1: {
		# 			'surf': {'panel':view, 'btn':view_btn, 'surf': view_surf},
		# 			'offs': view_offs
		# 		}
		# 	}
		# }
		self.surf = {
			'cam': {},
			'view': {},
			
			'cam_offs_ori': [10, 10],
			'cam_offs_cur': [10, 70],
			'cam_dimens': [500, HEIGHT ],
			'cam_pad': [10, 40],
			
			'view_offs_ori': [530, 50],
			'view_offs_cur': [10, 10],
			'view_dimens': [430, HEIGHT-70],
			'view_pad': [10, 10],
		}
		self.var = UI_ADMIN_VAR()


		self.cam_panel = pygame_gui.elements.UIPanel(
			relative_rect=((0,0), self.surf['cam_dimens']),
			manager=self.man
		)
		self.cam_label = pygame_gui.elements.UILabel(
			relative_rect=pygame.Rect((30, 10), (400, 50)),
			text="Cam Panel",
			manager=self.man
		)

		self.view_panel = pygame_gui.elements.UIScrollingContainer(
			relative_rect=pygame.Rect((self.surf['view_offs_ori'], self.surf['view_dimens'])),
			manager=self.man,
			allow_scroll_x=False
		)
		self.view_label = pygame_gui.elements.UILabel(
			relative_rect=pygame.Rect((530, 10), (400, 50)),
			text="View Panel",
			manager=self.man
		)


		# Cam Control Window
		self.cam_w_control_btn = pygame_gui.elements.UIButton(
			relative_rect=pygame.Rect((280, 24), (30, 20)),
			text="...", container=self.cam_panel, manager=self.man
		)
		self.w_cam_ctrl = self.w_view_ctrl = {
			'win': pygame_gui.elements.UIWindow(
				rect=pygame.Rect((WIDTH/4, HEIGHT/2), (204, HEIGHT/4)),
				window_display_title="Cam Control"
			),
			'btn_stop': {},
			'btn_del': {},
			'label_func_sel_cam': {},

			'list_func': None
		}
		self.w_cam_ctrl['btn_stop'] = pygame_gui.elements.UIButton(
			relative_rect=pygame.Rect((10,0),(90, 40)), container=self.w_cam_ctrl['win'],
			text="Stop/Play"
		)
		self.w_cam_ctrl['btn_del'] = pygame_gui.elements.UIButton(
			relative_rect=pygame.Rect((100,0),(90, 40)), container=self.w_cam_ctrl['win'],
			text="Delete"
		)
		self.w_cam_ctrl['label_func_sel_cam'] = pygame_gui.elements.UILabel(
			relative_rect=pygame.Rect((0,50),(200, 20)), container=self.w_cam_ctrl['win'],
			text="Select cam funced"
		)
		self.w_cam_ctrl['label_func_sel_cam'].hide()
		self.w_cam_ctrl['win'].hide()


		# View Control Window
		self.w_view_ctrl = {
			'win': pygame_gui.elements.UIWindow(
				rect=pygame.Rect((WIDTH/2, HEIGHT/2), (244, HEIGHT/4)),
				window_display_title="View Control"
			),
			'btn_del': {},
			'btn_sel_cam': {},
			'btns_sel_cam': {},
			'btn_del_view': {},
			'label_func_sel_cam': {},

			'view_selected': None,
		}
		self.w_view_ctrl['btn_sel_cam'] = pygame_gui.elements.UIButton(
			relative_rect=pygame.Rect((10,0),(100, 40)), container=self.w_view_ctrl['win'], 
			text="Select Cam"
		)
		self.w_view_ctrl['btn_del_view'] = pygame_gui.elements.UIButton(
			relative_rect=pygame.Rect((120,00),(100, 40)), container=self.w_view_ctrl['win'], 
			text="Delete View"
		)
		self.w_view_ctrl['label_func_sel_cam'] = pygame_gui.elements.UILabel(
			relative_rect=pygame.Rect((0,50),(244, 20)), container=self.w_view_ctrl['win'],
			text="Select cam link"
		)
		self.w_view_ctrl['label_func_sel_cam'].hide()
		self.w_view_ctrl['win'].hide()

	def update(self, dtime):
		self.man.update(dtime)

		# Show/Hide button cam control window
		if self.cam_label.check_hover(dtime, False): 
			self.cam_w_control_btn.show()
		else:
			self.cam_w_control_btn.hide()
		
		pass

	def process_events(self, event):

		if event.type == pygame_gui.UI_BUTTON_PRESSED: 

			# Process window cam control
			if event.ui_element == self.cam_w_control_btn:
				self.w_cam_ctrl['win'].show()
				self.w_cam_ctrl['label_func_sel_cam'].hide()

			if event.ui_element == self.w_cam_ctrl['btn_stop']:
				self.var.func_sel_cam = 'stopped'
				self.w_cam_ctrl['label_func_sel_cam'].set_text("Select cam stopped/played")
				self.w_cam_ctrl['label_func_sel_cam'].show()
				self.show_btns_sel_cam()

			if event.ui_element == self.w_cam_ctrl['btn_del']:
				self.var.func_sel_cam = 'deleted'
				self.w_cam_ctrl['label_func_sel_cam'].set_text("Select cam deleted")
				self.w_cam_ctrl['label_func_sel_cam'].show()
				self.show_btns_sel_cam()
			#=========================

			# Process click view item
			for id, obj in self.surf['view'].items():
				if event.ui_element == obj['surf']['btn']:
					win = self.w_view_ctrl['win']
					if not win.visible: win.show()
					self.w_view_ctrl['label_func_sel_cam'].hide()

					link_cam = None
					if id in self.app.man['view'].keys(): 
						link_cam = self.app.man['view'][id]
					win.set_display_title(f"View Control (link cam-{link_cam})")
					self.w_view_ctrl['view_selected'] = id
			#=========================
			
			# Process window view control
			for id, obj in self.w_view_ctrl['btns_sel_cam'].items():
				if event.ui_element == obj:
					if self.var.func_sel_cam == 'stopped':
						self.func_sel_cam_stopped(id)
					elif self.var.func_sel_cam == 'deleted':
						self.func_sel_cam_deleted(id)
					elif self.var.func_sel_cam == 'link':
						self.func_sel_cam_link(id)

					self.var.func_sel_cam = None
					self.hide_btns_sel_cam()

			if event.ui_element == self.w_view_ctrl['btn_sel_cam']:
				self.show_btns_sel_cam()
				self.w_view_ctrl['label_func_sel_cam'].set_text("Select cam linked")
				self.w_view_ctrl['label_func_sel_cam'].show()
				self.var.func_sel_cam = 'link'
			#============================

			# Process click delete button in view control
			if event.ui_element == self.w_view_ctrl['btn_del_view']:
				self.w_view_ctrl['win'].hide()
				# print(f"Del view {self.w_view_ctrl['view_selected']}")
				json_str = json.dumps({'mes': 'del_view', 'data': self.w_view_ctrl['view_selected']})
				self.app.io_put(tcp_pack(DT.STR, json_str))
			#============================

		self.man.process_events(event)

	def add_cam(self, id):
		cam_offs = copy.copy(self.surf['cam_offs_cur'])
		cam_dimens = [240, 240]

		cam =  pygame_gui.elements.UIImage (
			relative_rect=(cam_offs, cam_dimens),
			image_surface=self.assets.default['no_signal'],
			container=self.cam_panel,
			manager=self.man
		)
		self.surf['cam'][id] = {'surf':cam, 'offs': cam_offs}
		self.surf['cam'][id]['surf_no_signal'] = self.assets.default['no_signal']
		
		self.surf['cam_offs_cur'][0] += (cam_dimens[0] + self.surf['cam_pad'][0])
		if self.surf['cam_offs_cur'][0] > self.surf['cam_dimens'][0] - cam_dimens[0]:
			self.surf['cam_offs_cur'][0] = self.surf['cam_offs_ori'][0]
			self.surf['cam_offs_cur'][1] += (cam_dimens[1] + self.surf['cam_pad'][1])

	def remove_cam(self, id):
		cam_obj = self.surf['cam'][id]

		# Update cam offset current
		cam_offs_cur = self.surf['cam_offs_cur']
		if cam_offs_cur[0] > cam_obj['offs'][0] or cam_offs_cur[1] > cam_obj['offs'][1]:
			self.surf['cam_offs_cur'] = cam_obj['offs']
		
		# Delete cam surface, cam object
		cam_obj['surf'].kill()
		del self.surf['cam'][id]

	def add_view(self, id):
		view_offs = copy.copy(self.surf['view_offs_cur'])
		view_dimens = [200, 200]

		view = pygame_gui.elements.UIPanel(
			relative_rect=(view_offs, view_dimens),
			container=self.view_panel,
			manager=self.man
		)
		view_btn = pygame_gui.elements.UIButton (
			relative_rect=pygame.Rect((50, 0), (100, 20)),
			text=f'View-{id}',
			container=view,
			manager=self.man
		)
		view_surf = pygame_gui.elements.UIImage (
			relative_rect=((30,30), (140, 140)),
			image_surface=self.assets.default['no_signal'],
			container=view,
			manager=self.man
		)
		self.surf['view'][id] = {
			'surf': {'panel':view, 'btn':view_btn, 'surf': view_surf},
			'offs': view_offs
		}

		self.surf['view_offs_cur'][0] += (view_dimens[0] + self.surf['view_pad'][0])
		if self.surf['view_offs_cur'][0] > self.surf['view_dimens'][0] - view_dimens[0]:
			self.surf['view_offs_cur'][0] = 10
			self.surf['view_offs_cur'][1] += (view_dimens[1] + self.surf['view_pad'][1])
		
	def remove_view(self, id):
		view_obj = self.surf['view'][id]

		# Update view offet current
		view_offs_cur = self.surf['view_offs_cur']
		if view_offs_cur[0] > view_obj['offs'][0] or view_offs_cur[1] > view_obj['offs'][1]:
			self.surf['view_offs_cur'] = view_obj['offs']
		
		# Delete view panel, view object
		view_obj['surf']['panel'].kill()
		del self.surf['view'][id]


	def update_cam(self, id, surf):
		if id not in self.surf['cam'].keys(): return
		
		cam_obj = self.surf['cam'][id]
		if surf:
			cam_obj['surf'].set_image(surf)
		else:
			cam_obj['surf'].set_image(cam_obj['surf_no_signal'])
	
	def update_view(self, id, surf):
		if id not in self.surf['view'].keys(): return
		
		view_surf = self.surf['view'][id]['surf']
		if surf:
			view_surf['surf'].set_image(surf)
		else:
			view_surf['surf'].set_image(self.assets.default['no_signal'])

	def show_btns_sel_cam(self):
		if len(self.w_view_ctrl['btns_sel_cam']) != 0:
			self.hide_btns_sel_cam()

		for id in self.app.man['cam']:
			print(f"rect: {self.surf['cam'][id]['surf'].get_relative_rect()}")
			cam_rect = self.surf['cam'][id]['surf'].get_relative_rect()
			btn_sel_rect = pygame.Rect(
				(cam_rect[0], cam_rect[1]+cam_rect[3]), (80, 30)
			)
			btn_sel = pygame_gui.elements.UIButton(
				relative_rect=btn_sel_rect, text="Select",
				manager=self.man, container=self.cam_panel
			)
			self.w_view_ctrl['btns_sel_cam'][id] = btn_sel

	def hide_btns_sel_cam(self):
		for obj in self.w_view_ctrl['btns_sel_cam'].values():
			obj.kill()
		self.w_view_ctrl['btns_sel_cam'] = {}

	def func_sel_cam_stopped(self, id_cam):
		if id_cam in self.app.man['cam_stopped'].keys():
			json_str = json.dumps({'mes': 'transmit_cam', 'data': f"{id_cam}-0"})
			self.app.io_put(tcp_pack(DT.STR, json_str))
			del self.app.man['cam_stopped'][id_cam]
		else:
			json_str = json.dumps({'mes': 'transmit_cam', 'data': f"{id_cam}-1"})
			self.app.io_put(tcp_pack(DT.STR, json_str))
			self.app.man['cam_stopped'][id_cam] = 1

	def func_sel_cam_deleted(self, id_cam):
		json_str = json.dumps({'mes': 'del_cam', 'data': id_cam})
		self.app.io_put(tcp_pack(DT.STR, json_str))

	def func_sel_cam_link(self, id_cam):
		view_sel = self.w_view_ctrl['view_selected']
		json_str = json.dumps({'mes': 'change_view_link', 'data': f"{id_cam}-{view_sel}"})
		self.app.io_put(tcp_pack(DT.STR, json_str))

	def draw(self, on_surf):
		self.man.draw_ui(on_surf)