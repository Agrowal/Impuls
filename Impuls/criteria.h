#ifndef CRITERIA_H
#define CRITERIA_H


class Criteria
{
public:
    Criteria(char comparisonOperator,
             char column,
             char value);

    void setComparisonOperator(char newComparisonOperator);
    void setColumn(char newColumn);
    void setValue(char newValue);

private:
    char comparisonOperator;
    char column;
    char value;

};

#endif // CRITERIA_H
