servo
=====

read servo using the RaduinoCommandTimer command
write servo using the RaduinoCommandPwm command

raduino-servo
-------------

servos for RC is controlled by a pwm signal, 50Hz and pulse width ranging from 1-2 ms. The command is sending the value in 10us increments. Thus the minimum value of 1.0 ms is encoded as the integer 100

.. code-block:: sh

    /bin/raduino-servo -s 100

