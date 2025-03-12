## About the project
This is a little cube toy with leds that changes color depending on which side it's laid on. It uses accelerometer to detect orientation and if you shake it, it does a random color animation. With 350mAh battery it lasts for about 2 weeks depending on use.  

![alt text](https://raw.githubusercontent.com/mcer12/CuteCube/refs/heads/main/Images/showcase.gif)

## Important notes
- you can use BMI055 or BMI088, make sure to select correct one in the sketch as they have different instruction sets
- Use my fork of the libraries to allow low power (gyro sleep):  
BMI088: https://github.com/mcer12/bmi088-arduino  
BMI055: https://github.com/mcer12/FastIMU  
- If you want to enable easy flashing via USB (serial), you need to set nBOOT_SEL fuse to 0: BOOT0 signal is defined by BOOT0 pin value (legacy mode)
- Forgive the code quality, it was a quick arduino project, not meant to be published and I'm NOT cleaning that up :D

## Printing notes
- No supports needed
- Transparent PLA or PETG highly recommended.
- Two size variants (55x55mm and 67x67mm)
- Requires two long M3 bols and two hot melt inserts.
- Bolts: I used 25mm 
- Inserts: OD4.5mm, 10mm length. You don't need to use inserts this long but they make it pretty much indestructible.
