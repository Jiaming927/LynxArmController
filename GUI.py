from Tkinter import *
from subprocess import call
import time

class App:
	def __init__(self, master):
		Label(master, text="#Servo:    ").grid(row=0, column = 1)
		Label(master, text="Speed:     ").grid(row=1, column = 1)
		Label(master, text="Position:  ").grid(row=2, column = 1)

		self.smoothSpeed = StringVar()
		self.smoothPosition = StringVar()

		self.servoVal = StringVar()
		self.servoVal.set("pick one")

		self.servo = OptionMenu(master, self.servoVal, "pick one", "#0", "#1", "#2", "#3")
		self.servo.grid(row=0, column=2, padx=5, pady=2.5)

		self.speedEntry = Entry(master, width = 4, textvariable=self.smoothSpeed)
		self.speed = Scale(master, from_=0, to=100, orient=HORIZONTAL)
		self.speed.grid(row=1, column=2, padx=5, pady=2.5)
		self.speedEntry.grid(row=1, column=3, padx=5, pady=1)

		self.positionEntry = Entry(master, width = 4, textvariable=self.smoothPosition)
		self.position = Scale(master, from_=0, to=100, orient=HORIZONTAL, command=self.position_change)
		self.position.grid(row=2, column=2, padx=5, pady=2.5)
		self.positionEntry.grid(row=2, column=3, padx=5, pady=1)

		button = Button(master, text="QUIT", fg="red", command=master.quit)
		button.grid(row=3, column=1, padx=10, pady=2.5)

		ModeButton = Button(master, text="Move Smoothly", fg="blue", command=self.smooth_position_change)
		ModeButton.grid(row=3, column=3, padx=10, pady=2.5)

	def position_change(self, position):
		temp = self.servoVal.get()
		index = temp[1]
		speed = int(self.speed.get()) * 100
		pos = int(position) * 20 + 500
		print index, pos, speed
		# print index
		# print speed
		# print pos
		# send index position speed
		call(["rosrun", "arm_controller", "arm_controller_client", index, str(pos), str(speed)])

	def smooth_position_change(self):
		print "Smooth Mode"
		temp = self.servoVal.get()
		index = temp[1]
		speed = int(self.smoothSpeed.get()) * 100
		pos = int(self.smoothPosition.get()) * 20 + 500
		print index, pos, speed
		# print index
		# print speed
		# print pos
		# send index position speed
		call(["rosrun", "arm_controller", "arm_controller_client", index, str(pos), str(speed)])

root = Tk()
root.title("Arm Controller")
app = App(root)
root.mainloop()

