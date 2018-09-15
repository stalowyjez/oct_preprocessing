#include <string>
#include <vector>
#include <gmp.h>
#include <gmpxx.h>

#ifndef FIELD_H

#define FIELD_H

namespace Field {
	class Element : public mpz_class {
		public:
		Element inv();

		Element();
		Element(std::string);
		Element(int);
		Element(mpz_t);
		Element(mpz_class n);

		void print();
	};
}

Field::Element operator+(Field::Element A, Field::Element B);
Field::Element operator-(Field::Element A, Field::Element B);
Field::Element operator*(Field::Element A, Field::Element B);
Field::Element operator/(Field::Element A, Field::Element B);

typedef std::vector<std::vector<Field::Element>> Matrix;

#endif
