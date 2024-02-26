//
//    Copyright 2024 Christopher D. McMurrough
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

int buttonState = 0;
int counter = 0;

/***********************************************************************************************************************
 * @BRIEF perform initial setup of the microcontroller
 * @AUTHOR Christoper D. McMurrough
 **********************************************************************************************************************/
void setup()
{
    // initialize LEDs
    pinMode(LED_USER, OUTPUT);
    pinMode(LED_D0, OUTPUT);
    pinMode(LED_D1, OUTPUT);
    pinMode(LED_D2, OUTPUT);
    pinMode(LED_D3, OUTPUT);

    // initialize relays
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);

    // initialize button
    pinMode(BTN_USER, INPUT);
}

/***********************************************************************************************************************
 * @BRIEF main program loop
 * @AUTHOR Christoper D. McMurrough
 **********************************************************************************************************************/
void loop()
{
    digitalWrite(LED_USER, HIGH);
    buttonState = digitalRead(BTN_USER);
    if(buttonState == LOW)
    {
        if(counter < 4)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }
        delay(100);
    }
    toggleRelays();
}

/***********************************************************************************************************************
 * @BRIEF toggle LEDs and relays
 * @AUTHOR Christoper D. McMurrough
 **********************************************************************************************************************/
void toggleRelays()
{
    switch(counter)
    {
    case 0:
        digitalWrite(LED_D0, LOW);
        digitalWrite(LED_D1, LOW);
        digitalWrite(LED_D2, LOW);
        digitalWrite(LED_D3, LOW);
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
        break;
    case 1:
        digitalWrite(LED_D0, HIGH);
        digitalWrite(LED_D1, LOW);
        digitalWrite(LED_D2, LOW);
        digitalWrite(LED_D3, LOW);
        digitalWrite(D0, HIGH);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
        break;
    case 2:
        digitalWrite(LED_D0, LOW);
        digitalWrite(LED_D1, HIGH);
        digitalWrite(LED_D2, LOW);
        digitalWrite(LED_D3, LOW);
        digitalWrite(D0, LOW);
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
        break;
    case 3:
        digitalWrite(LED_D0, LOW);
        digitalWrite(LED_D1, LOW);
        digitalWrite(LED_D2, HIGH);
        digitalWrite(LED_D3, LOW);
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, LOW);
        break;
    case 4:
        digitalWrite(LED_D0, LOW);
        digitalWrite(LED_D1, LOW);
        digitalWrite(LED_D2, LOW);
        digitalWrite(LED_D3, HIGH);
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, HIGH);
        break;
    }
    delay(100);
}
