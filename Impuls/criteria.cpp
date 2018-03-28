#include "criteria.h"

Criteria::Criteria(char comparisonOperator,
                   char column,
                   char value) :
    comparisonOperator(comparisonOperator),
    column(column),
    value(value)
{

}

void Criteria::setComparisonOperator(char newComparisonOperator){
    comparisonOperator = newComparisonOperator;
}
void Criteria::setColumn(char newColumn){
    column = newColumn;
}
void Criteria::setValue(char newValue){
    value = newValue;
}
