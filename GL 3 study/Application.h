#ifndef APPLICATION_H
#define	APPLICATION_H

enum class StatusApp : bool
{
  Finished = false,
  Worked = true
};

class Application
{
public:
  static StatusApp status;
};

#endif /*APPLICATION_H*/