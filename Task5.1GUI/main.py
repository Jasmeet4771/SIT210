import tkinter as tkin
import RPi.GPIO as GPIO


# Here i have setuped the GPIO pins
# this GPIO.BCM means that i have declared the pin numbers acc to Broadcom GPIO numbers.
GPIO.setmode(GPIO.BCM)

# here i defined each pin for each light
LIVING_ROOM = 17
BATHROOM = 22
CLOSET = 26

# here i did the GPIO pin setup 
GPIO.setup(LIVING_ROOM, GPIO.OUT)
GPIO.setup(BATHROOM, GPIO.OUT)
GPIO.setup(CLOSET, GPIO.OUT)

# at start all LEDs are OFF
GPIO.output(LIVING_ROOM, GPIO.LOW)
GPIO.output(BATHROOM, GPIO.LOW)
GPIO.output(CLOSET, GPIO.LOW)



# this is for light control
def change_room():
    # This turn all LEDs OFF
    GPIO.output(LIVING_ROOM, GPIO.LOW)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    # This turn ON the selected room
    selected_room = room.get()


    if selected_room == "Living Room":
        GPIO.output(LIVING_ROOM, GPIO.HIGH) #here it turns on the light. if room is living room

    elif selected_room == "Bathroom":
        GPIO.output(BATHROOM, GPIO.HIGH) #here it turns on the light. if room is Bathroom

    elif selected_room == "Closet":
        GPIO.output(CLOSET, GPIO.HIGH) #here it turns on the light. if room is Closet


# simple exit function for exit button turns all light off and then exits
def exit_gui():
    GPIO.output(LIVING_ROOM, GPIO.LOW)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    window.destroy()


# here comes the creation of window.
window = tkin.Tk()
window.title("Smart Home Lighting") # title
window.geometry("400x350")


# Heading
title = tkin.Label(
    window,
    text="Smart Home Lighting",
    font=("Arial", 20, "bold")
)

title.pack(pady=20) # now this puts the title and pady is for padding in y axis


# Variable used by the radio buttons
room = tkin.StringVar()
room.set("None")


# radio buttons
# living room
living_button = tkin.Radiobutton(
    window,
    text="Living Room",
    variable=room,
    value="Living Room",
    command=change_room,
    font=("Arial", 14)
)

living_button.pack(pady=5)

#bathroom
bathroom_button = tkin.Radiobutton(
    window,
    text="Bathroom",
    variable=room,
    value="Bathroom",
    command=change_room,
    font=("Arial", 14)
)

bathroom_button.pack(pady=5)

# closet
closet_button = tkin.Radiobutton(
    window,
    text="Closet",
    variable=room,
    value="Closet",
    command=change_room,
    font=("Arial", 14)
)

closet_button.pack(pady=5)

# exit
exit_button = tkin.Button(
    window,
    text="Exit",
    command=exit_gui,
    font=("Arial", 14),
    width=12
)

exit_button.pack(pady=25)


# ---------------- RUN GUI ----------------

try:
    window.mainloop()

finally:
    GPIO.cleanup()