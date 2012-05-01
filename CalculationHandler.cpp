#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include "CalculationHandler.hpp"
#include "enum.hpp"
#include "Menus.hpp"

using std::cout;
using std::string;
using std::cin;
using std::setprecision;
using std::endl;
using std::ios;
using std::fixed;

extern MODES currentModes;
extern OPTIONS currentOptions;

void sci_calculator(SciCalcParser& calcParser)
{
    string exp;
    getline(cin, exp);
    
    if (exp[0] != 'm' && exp[0] != 'q' &&
        exp.find("menu") == string::npos && exp.find("quit") == string::npos)
    {
        calcParser.setExpression(exp);
        string result = calcParser.evaluateExpression();
        cout << atof(result.c_str()) << endl;
    }
    else
    {
        cout << endl;
        
        if (exp[0] == 'm' || exp.find("menu") != string::npos)
        {
            currentModes.mainMode = MENU;
            currentModes.subMode = NONE;
        }
        else
        {
            currentModes.mainMode = EXIT;
        }
    }
}

void equivalent_component(EquivComponentParser& eqComponentParser)
{
    if (currentModes.subMode == NONE)
    {
        eqComponentSubmenu();
        return;
    }
    
    //Depending on the sub mode, perform calculations.
    string exp;
    getline(cin, exp);
      
    if (exp[0] != 'm' && exp[0] != 'q' &&
        exp.find("menu") == string::npos && exp.find("quit") == string::npos)
    {
        eqComponentParser.setMode(currentModes.subMode);
        eqComponentParser.setExpression(exp);
        string result = eqComponentParser.evaluateExpression();
        cout << atof(result.c_str());
        
        switch (currentModes.subMode)
        {
            case RESISTANCE:
                cout << " ohms"  << endl;
                break;
            case CAPACITANCE:
                cout << " F"  << endl;
                break;
            case INDUCTANCE:
                cout << " H" << endl;
                break;
        }
    }
    else
    {
        cout << endl;
        
        if (exp[0] == 'm' || exp.find("menu") != string::npos)
        {
            currentModes.mainMode = MENU;
            currentModes.subMode = NONE;
        }
        else
        {
            currentModes.mainMode = EXIT;
        }
    } 
}

void divider()
{
    cout << "Enter the value of the source in Volts. Enter \"m\" or \"q\" to return to the main menu or quit." << endl;
    string exp;
    getline(cin, exp);

    if (exp[0] != 'm' && exp[0] != 'q' &&
        exp.find("menu") == string::npos && exp.find("quit") == string::npos)
    {
        Divider divider;
        double sourceVal, r1, r2;
        
        sourceVal = strtod(exp.c_str(), NULL);

        cout << "Enter the value of the first resistor in ohms" << endl;
        getline(cin, exp);

        r1 = strtod(exp.c_str(), NULL);

        cout << "Enter the value of the second resistor in ohms" << endl;
        getline(cin,exp);

        r2 = strtod(exp.c_str(), NULL);

        double result1, result2;

        if (currentModes.subMode == CURRENT)
        {
            divider.divideCurrent(sourceVal, r1, r2, &result1, &result2);
            cout << "The current through the first resistor is " << result1 << " amps." << endl
                 << "The current through the second resistor is " << result2 << " amps." << endl;
        }
        else if (currentModes.subMode == VOLTAGE)
        {
            divider.divideVoltage(sourceVal, r1, r2, &result1, &result2);
            cout << "The voltage across the first resistor is " << result1 << " volts." <<endl
                 << "The voltage across the second resistor is " << result2 << " volts." <<endl;
        }
    }
    else
    {
        cout << endl;
        
        if (exp[0] == 'm' || exp.find("menu") != string::npos)
        {
            currentModes.mainMode = MENU;
            currentModes.subMode = NONE;
        }
        else
        {
            currentModes.mainMode = EXIT;
        }
    }
}

void options(SciCalcParser& calcParser)
{    
    string choice;
    
    do
    {
        cout << "Current Options: Enter the respective number to toggle." << endl
             << "1. Angle Units: " << ((currentOptions.angleMode == RADIAN) ? "radians" : "degrees") << endl
             << "2. Help Tips: " << ((currentOptions.helpDisplay) ? "On" : "Off") << endl
             << "3. Result Precision: " << currentOptions.precision << endl
             << "0. Main Menu" << endl;
                
        getline(cin, choice);
        
        switch (choice[0])
        {
            case '1':
                currentOptions.angleMode = ((currentOptions.angleMode == RADIAN) ? DEGREE : RADIAN);
                calcParser.setAngleMode(currentOptions.angleMode);
                break;
            case '2':
                currentOptions.helpDisplay = !(currentOptions.helpDisplay);
                break;
            case '3':
            {
                string newPrecision;
                cout << "Enter the new precision, or d for default: ";
                getline(cin, newPrecision);
                if (newPrecision[0] == 'd')
                {
                    cout.unsetf(ios::fixed);
                    cout << setprecision(32);
                    currentOptions.precision = -1;
                    break;
                }
                int newPrec = atoi(newPrecision.c_str());
                if (newPrec <= 0 || newPrec > 15)
                {
                    cout << "Error: Precision must be between 1 and 15, inclusive." << endl;
                }
                else
                {
                    currentOptions.precision = newPrec;
                    cout << fixed << setprecision(currentOptions.precision);
                }
                break;
            }
            case '0':
                currentModes.mainMode = MENU;
                currentModes.subMode = NONE;
                break;
            default:
                cout << "Invalid choice" << endl;
                break;                
        }
        
    } while (choice[0] != '0');
}