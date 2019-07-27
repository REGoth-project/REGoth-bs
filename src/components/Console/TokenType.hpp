#pragma once

namespace REGoth
{
  enum class TokenType
  {
    Literal,
    Command,
    Instance,  // TODO: Yeah I am not so sure about this in relation to the original commands
    Waypoint,
    Freepoint,
  };
}  // namespace REGoth