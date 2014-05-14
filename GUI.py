from Tkinter import *
from subprocess import call

class App:
	def __init__(self, master):
		Label(master, text="#Servo:    ").grid(row=0, column = 1)
		Label(master, text="Speed:     ").grid(row=1, column = 1)
		Label(master, text="Position:  ").grid(row=2, column = 1)

		self.servoVal = StringVar()
		self.servoVal.set("#1")

		self.servo = OptionMenu(master, self.servoVal, "#1", "#2", "#3", "#4", "#5", "#6")
		self.servo.grid(row=0, column=2, padx=5, pady=2.5)

		self.speed = Scale(master, from_=0, to=100, orient=HORIZONTAL)
		self.speed.grid(row=1, column=2, padx=5, pady=2.5)

		self.position = Scale(master, from_=0, to=100, orient=HORIZONTAL, command=self.position_change)
		self.position.grid(row=2, column=2, padx=5, pady=2.5)

		button = Button(master, text="QUIT", fg="red", command=master.quit)
		button.grid(row=3, column=1, padx=10, pady=2.5)

	def position_change(self, position):
		str = self.servoVal.get()
		index = str[1]
		speed = int(self.speed.get()) * 20
		pos = int(position) * 20 + 500
		# print index
		# print speed
		# print pos
		# send index position speed
		call(["rosrun", "arm_controller", "arm_controller_client", "index", "speed", "pos"])

root = Tk()
root.title("Arm Controller")
app = App(root)
root.mainloop()

