/**
 * rafl: ExampleUtil.h
 * Copyright (c) Torr Vision Group, University of Oxford, 2015. All rights reserved.
 */

#ifndef H_RAFL_EXAMPLEUTIL
#define H_RAFL_EXAMPLEUTIL

#include <fstream>

#include <tvgutil/persistence/LineUtil.h>
#include <tvgutil/statistics/ProbabilityMassFunction.h>

#include "Example.h"

namespace rafl {

/**
 * \brief This class contains utility functions for examples.
 */
class ExampleUtil
{
  //#################### PUBLIC STATIC MEMBER FUNCTIONS ####################
public:
  /**
   * \brief Calculates the entropy of the label distribution of a set of examples.
   *
   * \param examples    The examples for whose label distribution we want to calculate the entropy.
   * \param multipliers Optional per-class ratios that can be used to scale the probabilities for the different labels.
   * \return            The entropy of the examples' label distribution.
   */
  template <typename Label>
  static float calculate_entropy(const std::vector<boost::shared_ptr<const Example<Label> > >& examples,
                                 const typename boost::mpl::identity<boost::optional<std::map<Label,float> > >::type& multipliers = boost::none)
  {
    return examples.empty() ? 0.0f : make_pmf(examples, multipliers).calculate_entropy();
  }

  /**
   * \brief Calculates the entropy of a label distribution represented by the specified histogram.
   *
   * \param histogram   The histogram.
   * \param multipliers Optional per-class ratios that can be used to scale the probabilities for the different labels.
   * \return            The entropy of the label distribution represented by the histogram.
   */
  template <typename Label>
  static float calculate_entropy(const tvgutil::Histogram<Label>& histogram,
                                 const typename boost::mpl::identity<boost::optional<std::map<Label,float> > >::type& multipliers = boost::none)
  {
    return histogram.empty() ? 0.0f : tvgutil::ProbabilityMassFunction<Label>(histogram, multipliers).calculate_entropy();
  }

  /**
   * \brief Loads a set of examples from the specified file.
   *
   * \param filename  The name of the file from which to load the examples.
   * \return          The loaded examples.
   */
  template <typename Label>
  static std::vector<boost::shared_ptr<const Example<Label> > > load_examples(const std::string& filename)
  {
    // FIXME: Make this robust to bad data.

    typedef boost::shared_ptr<const Example<Label> > Example_CPtr;
    std::vector<Example_CPtr> result;

    std::ifstream fs(filename.c_str());
    if(!fs) throw std::runtime_error("Error: '" + filename + "' could not be opened");

    const std::string delimiters(", ");
    std::vector<std::vector<std::string> > wordLines = tvgutil::LineUtil::extract_word_lines(fs, delimiters);

    for(size_t i = 0, lineCount = wordLines.size(); i < lineCount; ++i)
    {
      const std::vector<std::string>& words = wordLines[i];

      Descriptor_Ptr descriptor(new Descriptor);
      for(size_t j = 0; j < words.size() - 1; ++j)
      {
        descriptor->push_back(boost::lexical_cast<float>(words[j]));
      }

      Label label = boost::lexical_cast<Label>(words.back());
      result.push_back(Example_CPtr(new Example<Label>(descriptor, label)));
    }

    return result;
  }

  /**
   * \brief Makes a histogram from the label distribution of a set of examples.
   *
   * \param examples  The examples from whose label distribution we want to make a histogram.
   * \return          The histogram.
   */
  template <typename Label>
  static tvgutil::Histogram<Label> make_histogram(const std::vector<boost::shared_ptr<const Example<Label> > >& examples)
  {
    typedef boost::shared_ptr<const Example<Label> > Example_CPtr;

    tvgutil::Histogram<Label> histogram;
    for(typename std::vector<Example_CPtr>::const_iterator it = examples.begin(), iend = examples.end(); it != iend; ++it)
    {
      histogram.add((*it)->get_label());
    }

    return histogram;
  }

  /**
   * \brief Makes a probability mass function (PMF) from the label distribution of a set of examples.
   *
   * \param examples    The examples from whose label distribution we want to make a PMF.
   * \param multipliers Optional per-class ratios that can be used to scale the probabilities for the different labels.
   * \return            The PMF.
   */
  template <typename Label>
  static tvgutil::ProbabilityMassFunction<Label> make_pmf(const std::vector<boost::shared_ptr<const Example<Label> > >& examples,
                                                          const boost::optional<std::map<Label,float> >& multipliers = boost::none)
  {
    return tvgutil::ProbabilityMassFunction<Label>(make_histogram(examples), multipliers);
  }
};

}

#endif
