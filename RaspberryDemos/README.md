4 examples of applications generated from Dezyne running on a Raspberry Pi The examples use a "Joy-it Exp-500" shield as an easy way to have access to 4 leds and 4 buttons. The motor example uses a motor driver to have more power for the motor.

# Rasp1DemoButton
The most simple one. Just one button driving one led. The button is polled on a timer interrupt. The timer is instantiated from the Dezyne runtime.
Verum has a small video on request that guides you through all the steps of integrating the Dezyne code on the Raspberry. 

# Rasp2Demo4Leds
Similar to the first one but enhanced to 4 leds and 4 buttons.

# Rasp3DemoMotor
Uses the single pin at Raspberry that can be used to generate a PWM signal to drive a motor. Motor speeds up and down in 5 steps driven by an up and a down button. A third button changes the rotation direction Setting the target speed with the buttons and updating the actual speed is decoupled: With a seperate timer the motor follows the target speed. This also means that a change in direction goes in steps and may take some time

# Rasp4DemoMotorLed
Enhances the motor example, a separate component is responsible for showing the speed of the motor by turning one or more leds on
