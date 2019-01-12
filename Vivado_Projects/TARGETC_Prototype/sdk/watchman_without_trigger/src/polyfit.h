/*
 * polyfit.h
 *
 *  Created on: 11 janv. 2019
 *      Author: antho
 */

#ifndef SRC_POLYFIT_H_
#define SRC_POLYFIT_H_

int polyfit(const double* const dependentValues,
            const double* const independentValues,
            unsigned int        countOfElements,
            unsigned int        order,
            double*             coefficients);

#endif /* SRC_POLYFIT_H_ */
