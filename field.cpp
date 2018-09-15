#include <gmp.h>
#include <cstdlib>

#include <string>
#include <iostream>

#include "field.h"

using namespace std;

static mpz_class MOD("203956878356401977405765866929034577280193993314348263094772646453283062722701277632936616063144088173312372882677123879538709400158306567338328279154499698366071906766440037074217117805690872792848149112022286332144876183376326512083574821647933992961249 917319836219304274280243803104015000563790123");
//static mpz_class MOD("1009");
static gmp_randclass RAND_STATE(gmp_randinit_default);

Field::Element::Element() : mpz_class(RAND_STATE.get_z_range(MOD)) {}

Field::Element::Element(int n) : mpz_class(n) {}

Field::Element::Element(string n) : mpz_class(n) {}

Field::Element::Element(mpz_t n) : mpz_class(n) {}

Field::Element::Element(mpz_class n) : mpz_class(n) {}

Field::Element Field::Element::inv() {
	mpz_t _result;
	mpz_init(_result);
	mpz_invert(_result,this->get_mpz_t(),MOD.get_mpz_t());
	Field::Element result(_result);
	return result;
}

void Field::Element::print() {
	cout << (*this) << endl;
}

Field::Element operator+(Field::Element A, Field::Element B) {
	return Field::Element((dynamic_cast<mpz_class&>(A) + dynamic_cast<mpz_class&>(B)) % dynamic_cast<mpz_class&>(MOD));
}

Field::Element operator-(Field::Element A, Field::Element B) {
	return Field::Element((MOD + dynamic_cast<mpz_class&>(A) - dynamic_cast<mpz_class&>(B)) % dynamic_cast<mpz_class&>(MOD));
}

Field::Element operator*(Field::Element A, Field::Element B) {
	return Field::Element((dynamic_cast<mpz_class&>(A) * dynamic_cast<mpz_class&>(B)) % dynamic_cast<mpz_class&>(MOD));
}

Field::Element operator/(Field::Element A, Field::Element B) {
	return A * B.inv();
}

