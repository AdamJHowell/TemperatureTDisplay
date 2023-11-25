//
// Created by adam.howell on 2023-11-24.
//

#ifndef T_DISPLAY_S3_SHT40_FLOATARRAYMANAGER_H
#define T_DISPLAY_S3_SHT40_FLOATARRAYMANAGER_H

#include <Arduino.h>

/**
 * FloatArrayManager is a class meant to help averaging values.
 * The constructor takes an integer representing the array size.
 * Values can be set explicitly, or added sequentially.
 * The insertValue() method moves each value from its current index to index+1, then saves the new value to index 0.
 */
class FloatArrayManager
{
private:
  int arraySize;
  float *arrayValues;

public:
  /**
   * Constructor
   * @param size the number of elements the array should hold.
   */
  explicit FloatArrayManager( int size );

  // Destructor to free the allocated memory
  ~FloatArrayManager();

  void insertValue( float value );

  void setValue( int index, float value );

  // Function to get the average value in the array.
  float getAverage();

  // Function to get the maximum value in the array.
  float getMax();

  // Function to get the minimum value in the array.
  float getMin();

  void printValues();
};

#endif //T_DISPLAY_S3_SHT40_FLOATARRAYMANAGER_H
