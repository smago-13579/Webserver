#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <unistd.h> // sleep

// fork - 78
// execve - 82
// waitpid - 90


# define CLR_GRN "\033[0;32m"
# define CLR_YLW "\033[0;33m"
# define CLR_BLU "\033[0;34m"
# define CLR_RED "\033[0;31m"
# define CLR_PRP "\033[0;35m"
# define CLR_END "\033[0m"

# define PRT(x)			std::cout << x
# define PRINT(x)		std::cout << x << std::endl

# define ENDL			std::cout << std::endl;
# define PRT_BLU(x)		PRT(CLR_BLU << x << ": " << CLR_END);
# define PRINT_BLU(x)	PRINT(CLR_BLU << x << ":" << CLR_END << std::endl);

# define PRINT_TITLE(x)	ENDL PRINT_BLU(x)

void	ft_hello();

#endif