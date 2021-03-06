/***************************************************************************
 *   Copyright (C) 2018, 2019 by Fabiano Riccardi                          *
 *                                                                         *
 *   This file is part of Dimmable Light for Arduino                       *
 *                                                                         *
 *   Dimmable Light for Arduino is free software; you can redistribute     *
 *   it and/or modify it under the terms of the GNU Lesser General Public  *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/
#ifndef DIMMABLE_LIGHT_LINEARIZED_H
#define DIMMABLE_LIGHT_LINEARIZED_H

#include "Arduino.h"
#include "thyristor.h"

/**
 * This is the user-oriented DimmableLightLinearized class, 
 * a wrapper on Thyristor class. It differs from DimmableLight
 * "brightness" meaning: here the brightness it mapped linearly to
 * power delivered to your devices, in DimmableLight it is linearly mapped
 * to time point when thyristor is triggered.
 * The computation induced by this class may affect the performance of your MCU.
 */
class DimmableLightLinearized{
  public:
  	DimmableLightLinearized(int pin)
                  :thyristor(pin),brightness(0){
      if(nLights<N){   
        nLights++;
      }else{
        Serial.println("Max lights number reached, the light is not created!");
        // return error or exception
      }
    }

  	/**
   	 * Set the brightness, 0 to turn off the lamp
   	 */
  	void setBrightness(uint8_t bri){
#ifdef NETWORK_FREQ_50HZ
      double tempBrightness = -1.5034e-10*pow(bri,5)
               +9.5843e-08*pow(bri,4)
               -2.2953e-05*pow(bri,3)
               +0.0025471*pow(bri,2)
               -0.14965*bri
               +9.9846;
#elif defined(NETWORK_FREQ_60HZ)
      double tempBrightness = -1.2528e-10*pow(bri,5)
               +7.9866e-08*pow(bri,4)
               -1.9126e-05*pow(bri,3)
               +0.0021225*pow(bri,2)
               -0.12471*bri
               +8.3201;    
#endif
      tempBrightness *= 1000;

      thyristor.setDelay(tempBrightness);
    };

  	/**
  	 * Return the current brightness
  	 */
  	uint8_t getBrightness(){
  		return brightness;
  	}

  	/**
   	 * Turn off the light
   	 */
  	void turnOff(){
		  setBrightness(0);
  	}

  	~DimmableLightLinearized(){
      nLights--;
    }

  /**
   * Set the timer and the interrupt routine
   * Actually this function doesn't do nothing the first light is created 
   */
  static void begin(){
    Thyristor::begin();
  }

  /**
   * Set the pin dedicated to receive the AC zero cross signal
   */
  static void setSyncPin(uint8_t pin){
    Thyristor::setSyncPin(pin);
  }

  /**
   * Return the number of instantiated lights
   */
  static uint8_t getLightNumber(){
    return nLights;
  };

	private:
  static const uint8_t N = 8;
	static uint8_t nLights;

  Thyristor thyristor;
	
	/**
	 * Store the relative power delivered to your device
	 * Values range is [0;255]
	 */
	uint8_t brightness;
};

#endif // END DIMMABLE_LIGHT_LINEARIZED_H
