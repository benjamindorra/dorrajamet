// Generated by dia2code
#ifndef STATE__TITLE__H
#define STATE__TITLE__H

#include <string>

namespace state {

  /// class Title - 
  class Title {
    // Attributes
  protected:
    std::string id;
    std::string name;
    /// Pointer to the holder if any
    std::string holder;
    // Setters and Getters
    const std::string& getId() const;
    void setId(const std::string& id);
    const std::string& getName() const;
    void setName(const std::string& name);
    const std::string& getHolder() const;
    void setHolder(const std::string& holder);
  };

};

#endif
