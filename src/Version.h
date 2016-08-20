#pragma once

#ifndef __cplusplus
#error "This file must be compiled as C++."
#endif

#include "Hash.h"
#include "Optional.h"
#include "Value.h"

#include <arbiter/Version.h>

#include <functional>
#include <string>

struct ArbiterSemanticVersion final
{
  public:
    unsigned _major;
    unsigned _minor;
    unsigned _patch;

    Arbiter::Optional<std::string> _prereleaseVersion;
    Arbiter::Optional<std::string> _buildMetadata;

    ArbiterSemanticVersion (unsigned major, unsigned minor, unsigned patch, Arbiter::Optional<std::string> prereleaseVersion = Arbiter::Optional<std::string>(), Arbiter::Optional<std::string> buildMetadata = Arbiter::Optional<std::string>())
      : _major(major)
      , _minor(minor)
      , _patch(patch)
      , _prereleaseVersion(prereleaseVersion)
      , _buildMetadata(buildMetadata)
    {}

    /**
     * Attempts to parse a well-formed semantic version from a string.
     */
    // TODO: Add error reporting
    static Arbiter::Optional<ArbiterSemanticVersion> fromString (const std::string &versionString);

    bool operator== (const ArbiterSemanticVersion &other) const noexcept
    {
      return _major == other._major && _minor == other._minor && _patch == other._patch && _prereleaseVersion == other._prereleaseVersion && _buildMetadata == other._buildMetadata;
    }

    bool operator!= (const ArbiterSemanticVersion &other) const noexcept
    {
      return !(*this == other);
    }

    bool operator< (const ArbiterSemanticVersion &other) const noexcept;

    bool operator> (const ArbiterSemanticVersion &other) const noexcept
    {
      return other < *this;
    }

    bool operator>= (const ArbiterSemanticVersion &other) const noexcept
    {
      return !(*this < other);
    }

    bool operator<= (const ArbiterSemanticVersion &other) const noexcept
    {
      return other >= *this;
    }
};

std::ostream &operator<< (std::ostream &os, const ArbiterSemanticVersion &version);

struct ArbiterSelectedVersion final
{
  public:
    ArbiterSemanticVersion _semanticVersion;
    Arbiter::SharedUserValue _metadata;

    ArbiterSelectedVersion (ArbiterSemanticVersion semanticVersion, Arbiter::SharedUserValue metadata)
      : _semanticVersion(std::move(semanticVersion))
      , _metadata(std::move(metadata))
    {}

    bool operator== (const ArbiterSelectedVersion &other) const
    {
      return _semanticVersion == other._semanticVersion && _metadata == other._metadata;
    }

    bool operator!= (const ArbiterSelectedVersion &other) const
    {
      return !(*this == other);
    }

    bool operator< (const ArbiterSelectedVersion &other) const
    {
      return _semanticVersion < other._semanticVersion;
    }

    bool operator<= (const ArbiterSelectedVersion &other) const
    {
      return _semanticVersion <= other._semanticVersion;
    }

    bool operator> (const ArbiterSelectedVersion &other) const
    {
      return _semanticVersion > other._semanticVersion;
    }

    bool operator>= (const ArbiterSelectedVersion &other) const
    {
      return _semanticVersion >= other._semanticVersion;
    }
};

std::ostream &operator<< (std::ostream &os, const ArbiterSelectedVersion &version);

namespace std {

template<>
struct hash<ArbiterSemanticVersion> final
{
  public:
    size_t operator() (const ArbiterSemanticVersion &version) const
    {
      return Arbiter::hashOf(version._major)
        ^ Arbiter::hashOf(version._minor)
        ^ Arbiter::hashOf(version._patch)
        ^ Arbiter::hashOf(version._prereleaseVersion)
        ^ Arbiter::hashOf(version._buildMetadata);
    }
};

template<>
struct hash<ArbiterSelectedVersion> final
{
  public:
    size_t operator() (const ArbiterSelectedVersion &version) const
    {
      return Arbiter::hashOf(version._semanticVersion);
    }
};

} // namespace std
