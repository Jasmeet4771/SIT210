import tkinter as tkin
import RPi.GPIO as GPIO


# GPIO Pins
# Uses Broadcom GPIO numbering
GPIO.setmode(GPIO.BCM)

# here i defined GPIO pins for each light
LIVING_ROOM = 18      
BATHROOM = 13
CLOSET = 12

# here i set GPIO pins as outputs
GPIO.setup(LIVING_ROOM, GPIO.OUT)
GPIO.setup(BATHROOM, GPIO.OUT)
GPIO.setup(CLOSET, GPIO.OUT)



# Created PWM for the Living Room LED
# Frequency set to 100 Hz
living_pwm = GPIO.PWM(LIVING_ROOM, 100)

# Starts PWM at 0% brightness
living_pwm.start(0)

# Turns other LEDs OFF initially
GPIO.output(BATHROOM, GPIO.LOW)
GPIO.output(CLOSET, GPIO.LOW)


# this function changes room and also turns light on for that room
def change_room():

    # Turns Bathroom and Closet OFF
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    # this gets selected room
    selected_room = room.get()

    if selected_room == "Living Room":

        # Applies current brightness
        brightness = brightness_slider.get()
        living_pwm.ChangeDutyCycle(brightness)

    elif selected_room == "Bathroom":

        # Turns Living Room OFF
        living_pwm.ChangeDutyCycle(0)

        # Turns Bathroom ON
        GPIO.output(BATHROOM, GPIO.HIGH)

    elif selected_room == "Closet":

        # Turns Living Room OFF
        living_pwm.ChangeDutyCycle(0)

        # Turns Closet ON
        GPIO.output(CLOSET, GPIO.HIGH)



# function for changing brightness
def change_brightness(value):

    # Converts slider value to a number
    brightness = float(value)

    # Only controls brightness when Living Room is selected
    if room.get() == "Living Room":
        living_pwm.ChangeDutyCycle(brightness)


#Exit button

def exit_gui():

    # Turns all LEDs OFF
    living_pwm.ChangeDutyCycle(0)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    # Stops PWM
    living_pwm.stop()

    # Closes GUI
    window.destroy()


# gui window
window = tkin.Tk()

window.title("Smart Home Lighting")
window.geometry("400x450")


# heading for the window

title = tkin.Label(
    window,
    text="Smart Home Lighting",
    font=("Arial", 20, "bold")
)

title.pack(pady=20)


#room variable
room = tkin.StringVar()
room.set("None")


# living room button 
living_button = tkin.Radiobutton(
    window,
    text="Living Room",
    variable=room,
    value="Living Room",
    command=change_room,
    font=("Arial", 14)
)

living_button.pack(pady=5)


# bathroom button 
bathroom_button = tkin.Radiobutton(
    window,
    text="Bathroom",
    variable=room,
    value="Bathroom",
    command=change_room,
    font=("Arial", 14)
)

bathroom_button.pack(pady=5)


# closet button 
closet_button = tkin.Radiobutton(
    window,
    text="Closet",
    variable=room,
    value="Closet",
    command=change_room,
    font=("Arial", 14)
)

closet_button.pack(pady=5)


# slider part

# label part 
brightness_label = tkin.Label(
    window,
    text="Living Room Brightness",
    font=("Arial", 14, "bold")
)

brightness_label.pack(pady=(25, 5))

# slider
brightness_slider = tkin.Scale(
    window,
    from_=0,
    to=100,
    orient=tkin.HORIZONTAL,
    length=300,
    command=change_brightness
)

# Starts at 50% brightness
brightness_slider.set(50)

brightness_slider.pack(pady=10)


# exit button
exit_button = tkin.Button(
    window,
    text="Exit",
    command=exit_gui,
    font=("Arial", 14),
    width=12
)

exit_button.pack(pady=25)


# this starts gui
try:
    window.mainloop()

finally:
    living_pwm.stop()
    GPIO.cleanup()
