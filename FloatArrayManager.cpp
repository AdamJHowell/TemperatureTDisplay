#include "FloatArrayManager.h"

/**
 * Constructor.
 * @param size the number of elements the array should hold.
 */
FloatArrayManager::FloatArrayManager( int size )
{
  arraySize = size;
  arrayValues = new float[arraySize];
}

/**
 * Destructor to free the allocated memory.
 */
FloatArrayManager::~FloatArrayManager()
{
  delete[] arrayValues;
}

/**
 * Move each value from its current index to index+1, then save the new value to index 0.
 * @param value the value to insert into the array.
 */
void FloatArrayManager::insertValue( float value )
{
  for( int i = 1; i < arraySize; i++ )
  {
    // Copy an array value to the next "higher" slot.
    arrayValues[arraySize - i] = arrayValues[arraySize - i - 1];
  }
  // Insert the new value at the front of the array.
  arrayValues[0] = value;
}

/**
 * Set the value at a specific index.
 */
void FloatArrayManager::setValue( int index, float value )
{
  if( index >= 0 && index < arraySize )
  {
    arrayValues[index] = value;
  }
}

/**
 * Function to get the average value in the array.
 */
float FloatArrayManager::getAverage()
{
  float sum = 0.0;
  for( int i = 0; i < arraySize; i++ )
  {
    sum += arrayValues[i];
  }
  return sum / arraySize;
}

/**
 * Function to get the maximum value in the array.
 */
float FloatArrayManager::getMax()
{
  float maxValue = arrayValues[0];
  for( int i = 1; i < arraySize; i++ )
  {
    if( arrayValues[i] > maxValue )
    {
      maxValue = arrayValues[i];
    }
  }
  return maxValue;
}

/**
 * Function to get the minimum value in the array.
 */
float FloatArrayManager::getMin()
{
  float minValue = arrayValues[0];
  for( int i = 1; i < arraySize; i++ )
  {
    if( arrayValues[i] < minValue )
    {
      minValue = arrayValues[i];
    }
  }
  return minValue;
}

void FloatArrayManager::printValues()
{
  for( int i = 0; i < arraySize; i++ )
  {
    Serial.printf( "%d: %f\n", i, arrayValues[i] );
  }
}
