#ifndef COMRADE_CORELIB_PARALLELPORT
#define COMRADE_CORELIB_PARALLELPORT

namespace Comrade
{
	
namespace Corelib
{

class ParallelPort
{
	protected:
		unsigned char curoutput;
		unsigned char curinput;
		static const unsigned long BASEPORT=0x378;
	public:
		ParallelPort();
		~ParallelPort();
		void outputBit(int bitno, bool status);
		void outputByte(unsigned char byte);
		void refreshInput();
		bool inputBit(int bitno);
};
/* Bit numbers for input:
 * pin 10 - bit 3
 * pin 11 - bit 4
 * pin 12 - bit 2
 * pin 13 - bit 1
 * pin 15 - bit 0
 */

} // namespace Corelib

} // namespace Comrade
#endif
