#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define UNSET_BIT(A, B) ((A) &= ~(1 << (B)))
#define SET_BIT(A, B) ((A) |=  (1 << (B)))

#define SS_LOW UNSET_BIT(PORTB, SS)
#define SS_HIGH SET_BIT(PORTB, SS)

#endif
