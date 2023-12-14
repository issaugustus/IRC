NAME	=	ircserv
CC		=	c++
SRC		=	src/main.cpp src/Channel.cpp src/Client.cpp src/Message.cpp src/Server.cpp\
			src/cmd/info.cpp src/cmd/join.cpp src/Numeric/Error.cpp src/Numeric/Reply.cpp src/cmd/pass.cpp \
			src/cmd/user.cpp src/cmd/nick.cpp src/cmd/ping.cpp\
			src/cmd/topic.cpp src/cmd/invite.cpp src/cmd/kick.cpp src/cmd/join.cpp src/cmd/privmsg.cpp \
			src/cmd/part.cpp src/cmd/whois.cpp src/cmd/mode.cpp src/cmd/die.cpp src/cmd/oper.cpp \
			src/cmd/quit.cpp src/cmd/notice.cpp src/Bot.cpp src/cmd/kill.cpp src/cmd/pong.cpp
RM		=	rm -rf
CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g3
IFLAGS	=	-Iincludes/
OBJDIR	=	obj
OBJS	=	$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) ${IFLAGS} -c $< -o $@

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) ${IFLAGS} $^ -o $(NAME)

all:		$(NAME)

clean:
	$(RM) $(OBJDIR)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re