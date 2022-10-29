from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.core.window import Window
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.textinput import TextInput
from kivymd.uix.label import MDLabel
from kivymd.uix.screen import Screen
from kivymd.uix.button import MDFlatButton, MDRectangleFlatButton
from kivymd.uix.textfield import MDTextField

from kivy.lang import Builder
from kivymd.app import MDApp
from kivy.uix.screenmanager import Screen, ScreenManager

Window.clearcolor = (0, 0, 0, 1)  # For background color
Window.size = (360, 600)  # For size of the window

global kp_phi_value_final
global kd_phi_value_final
global ki_phi_value_final

global kp_theta_value_final
global kd_theta_value_final
global ki_theta_value_final

global kp_psi_value_final
global kd_psi_value_final
global ki_psi_value_final

global kp_z_value_final
global kd_z_value_final
global ki_z_value_final

kp_phi_value_final = 1
kd_phi_value_final = 2
ki_phi_value_final = 3

kp_theta_value_final = 4
kd_theta_value_final = 5
ki_theta_value_final = 6

kp_psi_value_final = 7
kd_psi_value_final = 8
ki_psi_value_final = 9

kp_z_value_final = 10
kd_z_value_final = 11
ki_z_value_final = 12

# screen_helper = '''
# MDBoxLayout:
#     orientation: "vertical"
#
#     MDTopAppBar:
#         title: "Demo App"
#         left_action_items: [["menu",lambda x: nav_drawer.set_state('toggle')]]
#         right_action_items: [["clock",lambda x: app.navigation_draw()]]
#         # elevation: 20
#     MDLabel:
#         text: "Hello World"
#         halign: "center"
#
#     MDBottomAppBar:
#         MDTopAppBar:
#             # title: "Help"
#             left_action_items: [["coffee",lambda x: app.navigation_draw()]]
#             mode: 'end'
#             type: 'bottom'
#             icon:  'language-python'
#             on_action_button: app.navigation_draw()
#             # right_action_items: [["clock",lambda x: app.navigation_draw()]]
#             # elevation: 20
#     MDNavigationDrawer:
#         id: nav_drawer
# '''

# navigation_helper = """
# NavigationLayout:
#     ScreenManager:
#         Screen:
#             BoxLayout:
#                 orientation: "vertical"
#                 MDTopAppBar:
#                     title: "Demo App"
#                     left_action_items: [["menu",lambda x: nav_drawer.toggle_nav_drawer()]]
#         MDNavigationDrawer:
#             id: nav_drawer
# """

screen_helper = """

ScreenManager:
    
    MainScreen:
        id: main_screen
        
    RollScreen:
        id: roll_screen
    PitchScreen:
        id: pitch_screen
    YawScreen:
        id: yaw_screen
    ZScreen:
        id: z_screen

<MainScreen>:
    name: 'main_screen'
    
    GridLayout:
        halign: "center"
        cols: 1
        row_force_default: True
        row_default_height: 50
        padding: 20
    
        
    
        MDRectangleFlatButton:
            text: 'Roll Gains'
            # pos_hint: {'center_x':0.5, 'center_y':0.5}
            on_press: root.manager.current = 'roll_screen'
    
        MDRectangleFlatButton:
            text: 'Pitch Gains'
            # pos_hint: {'center_x':0.5, 'center_y':0.5}
            on_press: root.manager.current = 'pitch_screen'

        MDRectangleFlatButton:
            text: 'Yaw Gains'
            # pos_hint: {'center_x':0.5, 'center_y':0.5}
            on_press: root.manager.current = 'yaw_screen'
            
        MDRectangleFlatButton:
            text: 'Z Gains'
            # pos_hint: {'center_x':0.5, 'center_y':0.5}
            on_press: root.manager.current = 'z_screen'
            
<RollScreen>:
    name: 'roll_screen'
    GridLayout:
        id:roll_screen_layout
        cols: 3
        row_force_default: True
        row_default_height: 50
        padding: "8dp"
    
        MDLabel:
            text: 'kp_phi'
            halign: 'center'
    
        MDTextField:
            id : entered_kp_phi
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_kp_phi_value_functon()

        MDLabel:
            text: 'kd_phi'
            halign: 'center'
    
        MDTextField:
            id : entered_kd_phi
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_kd_phi_value_functon()

        MDLabel:
            text: 'ki_phi'
            halign: 'center'
    
        MDTextField:
            id : entered_ki_phi

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_ki_phi_value_functon()

    MDRectangleFlatButton:
        text: 'Back'
        pos_hint: {'center_x':0.5, 'center_y':0.2}
        on_press: root.manager.current = 'main_screen'

<PitchScreen>:
    name: 'pitch_screen'
    GridLayout:
        id:pitch_screen_layout
        cols: 3
        row_force_default: True
        row_default_height: 50
        padding: "8dp"
    
        MDLabel:
            text: 'kp_theta'
            halign: 'center'
    
        MDTextField:
            id : entered_kp_theta
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                print("just click now")
                app.update_kp_theta_value_functon()

        MDLabel:
            text: 'kd_theta'
            halign: 'center'
    
        MDTextField:
            id : entered_kd_theta
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_kd_theta_value_functon()

        MDLabel:
            text: 'ki_theta'
            halign: 'center'
    
        MDTextField:
            id : entered_ki_theta

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_ki_theta_value_functon()

    MDRectangleFlatButton:
        text: 'Back'
        pos_hint: {'center_x':0.5, 'center_y':0.2}
        on_press: root.manager.current = 'main_screen'

<YawScreen>:
    name: 'yaw_screen'
    GridLayout:
        id:yaw_screen_layout
        cols: 3
        row_force_default: True
        row_default_height: 50
        padding: "8dp"
    
        MDLabel:
            text: 'kp_psi'
            halign: 'center'
    
        MDTextField:
            id : entered_kp_psi
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_kp_psi_value_functon()

        MDLabel:
            text: 'kd_psi'
            halign: 'center'
    
        MDTextField:
            id : entered_kd_psi
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_kd_psi_value_functon()

        MDLabel:
            text: 'ki_psi'
            halign: 'center'
    
        MDTextField:
            id : entered_ki_psi

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_ki_psi_value_functon()

    MDRectangleFlatButton:
        text: 'Back'
        pos_hint: {'center_x':0.5, 'center_y':0.2}
        on_press: root.manager.current = 'main_screen'

<ZScreen>:
    name: 'z_screen'
    GridLayout:
        id:z_screen_layout
        cols: 3
        row_force_default: True
        row_default_height: 50
        padding: "8dp"
    
        MDLabel:
            text: 'kp_z'
            halign: 'center'
    
        MDTextField:
            id : entered_kp_z
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_kp_z_value_functon()

        MDLabel:
            text: 'kd_z'
            halign: 'center'
    
        MDTextField:
            id : entered_kd_z
            # text : "1"

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_kd_z_value_functon()

        MDLabel:
            text: 'ki_z'
            halign: 'center'
    
        MDTextField:
            id : entered_ki_z

        MDRectangleFlatButton:
            text: 'Update'
            on_release:
                app.update_ki_z_value_functon()

    MDRectangleFlatButton:
        text: 'Back'
        pos_hint: {'center_x':0.5, 'center_y':0.2}
        on_press: root.manager.current = 'main_screen'

"""


class MainScreen(Screen):
    pass


class RollScreen(Screen):
    pass


class PitchScreen(Screen):
    pass


class YawScreen(Screen):
    pass


class ZScreen(Screen):
    pass


sm = ScreenManager()
sm.add_widget(MainScreen(name='main_screen'))
sm.add_widget(RollScreen(name='roll_screen'))
sm.add_widget(PitchScreen(name='pitch_screen'))
sm.add_widget(YawScreen(name='yaw_screen'))
sm.add_widget(ZScreen(name='z_screen'))


class DemoApp(MDApp):

    def build(self):
        global kp_phi_value_final
        global kd_phi_value_final
        global ki_phi_value_final

        global kp_theta_value_final
        global kd_theta_value_final
        global ki_theta_value_final

        global kp_psi_value_final
        global kd_psi_value_final
        global ki_psi_value_final

        global kp_z_value_final
        global kd_z_value_final
        global ki_z_value_final

        self.theme_cls.primary_palette = 'Red'
        self.screen = Builder.load_string(screen_helper)
        self.screen.ids.roll_screen.ids.entered_kp_phi.hint_text = str(kp_phi_value_final)
        self.screen.ids.roll_screen.ids.entered_kd_phi.hint_text = str(kd_phi_value_final)
        self.screen.ids.roll_screen.ids.entered_ki_phi.hint_text = str(ki_phi_value_final)

        self.screen.ids.pitch_screen.ids.entered_kp_theta.hint_text = str(kp_theta_value_final)
        self.screen.ids.pitch_screen.ids.entered_kd_theta.hint_text = str(kd_theta_value_final)
        self.screen.ids.pitch_screen.ids.entered_ki_theta.hint_text = str(ki_theta_value_final)

        self.screen.ids.yaw_screen.ids.entered_kp_psi.hint_text = str(kp_psi_value_final)
        self.screen.ids.yaw_screen.ids.entered_kd_psi.hint_text = str(kd_psi_value_final)
        self.screen.ids.yaw_screen.ids.entered_ki_psi.hint_text = str(ki_psi_value_final)

        self.screen.ids.z_screen.ids.entered_kp_z.hint_text = str(kp_z_value_final)
        self.screen.ids.z_screen.ids.entered_kd_z.hint_text = str(kd_z_value_final)
        self.screen.ids.z_screen.ids.entered_ki_z.hint_text = str(ki_z_value_final)

        return self.screen

    def navigation_draw(self):
        print("navigation")

    def update_kp_phi_value_functon(self):
        global kp_phi_value_final
        kp_phi_value_final = self.screen.ids.roll_screen.ids.entered_kp_phi.text
        self.screen.ids.roll_screen.ids.entered_kp_phi.hint_text = str(kp_phi_value_final)

    def update_kd_phi_value_functon(self):
        global kd_phi_value_final
        kd_phi_value_final = self.screen.ids.roll_screen.ids.entered_kd_phi.text
        self.screen.ids.roll_screen.ids.entered_kd_phi.hint_text = str(kd_phi_value_final)

    def update_ki_phi_value_functon(self):
        global ki_phi_value_final
        ki_phi_value_final = self.screen.ids.roll_screen.ids.entered_ki_phi.text
        self.screen.ids.roll_screen.ids.entered_ki_phi.hint_text = str(ki_phi_value_final)

    def update_kp_theta_value_functon(self):
        global kp_theta_value_final
        kp_theta_value_final = self.screen.ids.pitch_screen.ids.entered_kp_theta.text
        self.screen.ids.pitch_screen.ids.entered_kp_theta.hint_text = str(kp_theta_value_final)

    def update_kd_theta_value_functon(self):
        global kd_theta_value_final
        kd_theta_value_final = self.screen.ids.pitch_screen.ids.entered_kd_theta.text
        self.screen.ids.pitch_screen.ids.entered_kd_theta.hint_text = str(kd_theta_value_final)

    def update_ki_theta_value_functon(self):
        global ki_theta_value_final
        ki_theta_value_final = self.screen.ids.pitch_screen.ids.entered_ki_theta.text
        self.screen.ids.pitch_screen.ids.entered_ki_theta.hint_text = str(ki_theta_value_final)

    def update_kp_psi_value_functon(self):
        global kp_psi_value_final
        kp_psi_value_final = self.screen.ids.yaw_screen.ids.entered_kp_psi.text
        self.screen.ids.yaw_screen.ids.entered_kp_psi.hint_text = str(kp_psi_value_final)

    def update_kd_psi_value_functon(self):
        global kd_psi_value_final
        kd_psi_value_final = self.screen.ids.yaw_screen.ids.entered_kd_psi.text
        self.screen.ids.yaw_screen.ids.entered_kd_psi.hint_text = str(kd_psi_value_final)

    def update_ki_psi_value_functon(self):
        global ki_psi_value_final
        ki_psi_value_final = self.screen.ids.yaw_screen.ids.entered_ki_psi.text
        self.screen.ids.yaw_screen.ids.entered_ki_psi.hint_text = str(ki_psi_value_final)

    def update_kp_z_value_functon(self):
        global kp_z_value_final
        kp_z_value_final = self.screen.ids.z_screen.ids.entered_kp_z.text
        self.screen.ids.z_screen.ids.entered_kp_z.hint_text = str(kp_z_value_final)

    def update_kd_z_value_functon(self):
        global kd_z_value_final
        kd_z_value_final = self.screen.ids.z_screen.ids.entered_kd_z.text
        self.screen.ids.z_screen.ids.entered_kd_z.hint_text = str(kd_z_value_final)

    def update_ki_z_value_functon(self):
        global ki_z_value_final
        ki_z_value_final = self.screen.ids.z_screen.ids.entered_ki_z.text
        self.screen.ids.z_screen.ids.entered_ki_z.hint_text = str(ki_z_value_final)


DemoApp().run()
