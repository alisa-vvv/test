/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   get_next_line.c                                     :+:    :+:           */
/*                                                      +:+                   */
/*   By: avaliull <avaliull@student.codam.nl>                       +#+       */
/*                                                    +#+                     */
/*   Created: 2024/11/24 12:37:21 by avaliull       #+#    #+#                */
/*   Updated: 2024/12/02 20:18:24 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// TO DO: APPARENTLY READ ADDS MORE SYMBOls AFTTER FILE ENDS IF THE BUFFER IS TOO BIG (WHICH IS FUCKING DUMB AS FUCK, LITERALLY WHY)
// SO I AFTER EACH READ I SHOULD TRIM THE READ STRING AS FOLLOWING:
// 16 - READ_RE_VAL = AMOUNT OF CAHARACTERS TO ZERO FROM END OF NEXT_READ
// FUCKING SUPID SHIT I SPENT SO MCH TIME FIXING SOMETHING ELSE THI WAS THE PROBLEM RHTLAHFDHVFG
// wrong, it just doesnt overwrite anything. dumbass
// if i read 16 into a buffer of 20, index 15-19 are going to be garbage values. theyre just not overwritten bestie
// just how c is. the answer is c. its just c. dont check for null. ever.
// you think malloc cleans your fucking memory area????
// dumbass, nothing gets overwritten. its just saying 'here you are, heres some memory', you expect c to clean your fucking house as well?>


//TO DO: remove all cases where buff_str=[func] so they are just [func] (maybe except initial alloc)
// make all sub functions take a pointer to pointer to char, and pass the adress of it from get_next_line
// recheck the logic of the nonsense
// recheck the free checks

char	*rec_read(char **buff_str, int fd, int *next_line_end)
{
	char		next_read[BUFFER_SIZE + 1];
	int		read_re_val;
	int		i;
	int		buff_len;

	read_re_val = read(fd, next_read, BUFFER_SIZE);
	next_read[read_re_val] = '\0';
	if (read_re_val > 0)
		gnl_cat(buff_str, next_read, read_re_val);
	if (!(*buff_str))
		return (NULL);
	buff_len = gnl_strlen(*buff_str);
	i = 0;
	while ((*buff_str)[i] != '\0')
	{
		if ((*buff_str)[i] == '\n')
		{
			*next_line_end = i;
			return (*buff_str);
		}
		i++;
	}
	if (i == buff_len)
		rec_read(buff_str, fd, next_line_end);
	return (*buff_str);
}

static char	*find_nl(char **buff_str, int *next_line_end, int read_val, int fd)
{
	char	*next_line;
	int	i;

	while ((*buff_str)[*next_line_end] != '\0')
	{
		if ((*buff_str)[*next_line_end] == '\n')
			break ;
		(*next_line_end)++;
	}
	printf("*next_luine_end : %d\n", *next_line_end);
	if (*next_line_end == read_val)
		if (!rec_read(buff_str, fd, next_line_end))
			return (NULL);
	/* I think I have to put thuis part as a separate function and use it here and in the rec_read, returning either rec_read or this*/
	next_line = gnl_calloc(*next_line_end + 1);
	if (!next_line)
		return (NULL);
	i = 0;
	while (i <= *next_line_end)
	{
		next_line[i] = (*buff_str)[i];
		i++;
	}
	next_line[i] = '\0';
	if ((*buff_str)[i] == '\0')
		*next_line_end = -1;
	/* The pount is that rec_read would return a next_line by itself and clean the buffer properly. I think? this feels like nonsense */
	if (*next_line_end == -1) // get this shit outta here
		buff_zero(buff_str);
	else if (buff_str)
		trim_buff(buff_str, *next_line_end); // probably this as well
	return (next_line);
}

char	*get_next_line(int fd)
{
	static char	*buff_str;
	char		next_read[BUFFER_SIZE + 1];
	int		read_re_val;
	int		next_line_end;
	char		*next_line;

	read_re_val = read(fd, next_read, BUFFER_SIZE);
	next_read[read_re_val] = '\0';
	if (read_re_val > 0 && !buff_str)
		buff_str = alloc_buff(buff_str, next_read, read_re_val);
	else if (read_re_val <= 0 && !buff_str)
		return (NULL);
	else
		gnl_cat(&buff_str, next_read, read_re_val);
	if (!buff_str) // this should probably be w/o asterisk
		return (NULL);
	next_line_end = 0;
	next_line = find_nl(&buff_str, &next_line_end, read_re_val, fd);
	if (!next_line && buff_str)
	{
		buff_zero(&buff_str);
		return (NULL);
	}
	return (next_line);
}
