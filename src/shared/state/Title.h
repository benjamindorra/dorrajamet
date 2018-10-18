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
    // Operations
  public:
    virtual void debug () = 0;
    virtual ~Title () = default;
    virtual bool checkConsistency () = 0;
    virtual std::string getLiege () = 0;
    virtual std::string getDeJureLiege () = 0;
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
