/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medWidgets.h>

namespace medWidgets
{
namespace pluginManager
{
    void initialize(const QString& path, bool verbose)
    {
        for(QString const& realpath : path.split(';'))
        {
            if(realpath.isEmpty())
                break;
        }
    }
}
namespace morphomathOperation
{
    namespace openingImage
    {
        namespace _private
        {
            medAbstractOpeningImageProcessPresenterFactory factory;
        }

        medAbstractOpeningImageProcessPresenterFactory& presenterFactory()
        {
            return _private::factory;
        }
    }
}

namespace maskImage
{
    namespace _private
    {
        medAbstractMaskImageProcessPresenterFactory factory;
    }

    medAbstractMaskImageProcessPresenterFactory& presenterFactory()
    {
        return _private::factory;
    }
}

namespace dwiMasking
{
    namespace _private
    {
        medAbstractDWIMaskingProcessPresenterFactory factory;
    }

    medAbstractDWIMaskingProcessPresenterFactory& presenterFactory()
    {
        return _private::factory;
    }
}

namespace diffusionModelEstimation
{
    namespace _private
    {
        medAbstractDiffusionModelEstimationProcessPresenterFactory factory;
    }

    medAbstractDiffusionModelEstimationProcessPresenterFactory& presenterFactory()
    {
        return _private::factory;
    }
}

namespace diffusionScalarMaps
{
    namespace _private
    {
        medAbstractDiffusionScalarMapsProcessPresenterFactory factory;
    }

    medAbstractDiffusionScalarMapsProcessPresenterFactory& presenterFactory()
    {
        return _private::factory;
    }
}

namespace tractography
{
    namespace _private
    {
        medAbstractTractographyProcessPresenterFactory factory;
    }

    medAbstractTractographyProcessPresenterFactory& presenterFactory()
    {
        return _private::factory;
    }
}

}
