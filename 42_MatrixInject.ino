void matrixInject(int8_t pin, int8_t row, int8_t column)
{
	int8_t Pin = pin;
	int8_t Row = row - 1;
	int8_t Column = column - 1;

	rawState[Row][Column] = !digitalRead(Pin);  //将针脚读数取反后作为对应按钮的原始状态
}
