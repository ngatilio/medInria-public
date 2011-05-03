// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include "itkProcessRegistrationDiffeoMorphicDemons.h"

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractDataFactory.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

#include "itkImageRegistrationMethod.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkEuler3DTransform.h"
#include "itkCenteredTransformInitializer.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMetaImageIO.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkExtractImageFilter.h"

#include "itkCommand.h"

#include "time.h"

#include <DiffeomorphicDemons/rpiDiffeomorphicDemons.hxx>
#include <rpiCommonTools.hxx>

// /////////////////////////////////////////////////////////////////
// itkProcessRegistrationDiffeoMorphicDemonsDiffeoMorphicDemonsPrivate
// /////////////////////////////////////////////////////////////////

class itkProcessRegistrationDiffeoMorphicDemonsPrivate
{
public:
    itkProcessRegistrationDiffeoMorphicDemons * proc;
    template <class PixelType>
            int update(void);
    template < typename TFixedImage, typename TMovingImage >
           bool write(const QString&);
    void * registrationMethod ;
    void deleteRegMethod(void);
    std::vector<unsigned int> iterations;
    unsigned char updateRule;
    unsigned char gradientType;
    float maximumUpdateStepLength;
    float updateFieldStandardDeviation;
    float displacementFieldStandardDeviation;
    bool useHistogramMatching;
};

// /////////////////////////////////////////////////////////////////
// itkProcessRegistrationDiffeoMorphicDemons
// /////////////////////////////////////////////////////////////////

itkProcessRegistrationDiffeoMorphicDemons::itkProcessRegistrationDiffeoMorphicDemons(void) : itkProcessRegistration(), d(new itkProcessRegistrationDiffeoMorphicDemonsPrivate)
{
    d->proc = this;
    d->registrationMethod = NULL ;
    d->updateRule = 0;
    d->gradientType = 0;
    d->maximumUpdateStepLength = 2.0;
    d->updateFieldStandardDeviation = 0.0;
    d->displacementFieldStandardDeviation = 1.5;
    d->useHistogramMatching = false;
}

itkProcessRegistrationDiffeoMorphicDemons::~itkProcessRegistrationDiffeoMorphicDemons(void)
{
    d->proc = NULL;
    //d->deleteRegMethod();
    d->registrationMethod = NULL;
    delete d;
    d = NULL;
}

bool itkProcessRegistrationDiffeoMorphicDemons::registered(void)
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkProcessRegistrationDiffeoMorphicDemons",
              createitkProcessRegistrationDiffeoMorphicDemons);
}

QString itkProcessRegistrationDiffeoMorphicDemons::description(void) const
{
    return "itkProcessRegistrationDiffeoMorphicDemons";
}

// /////////////////////////////////////////////////////////////////
// Templated Version of update
// /////////////////////////////////////////////////////////////////


template <typename PixelType>
        int itkProcessRegistrationDiffeoMorphicDemonsPrivate::update(void)
{
    typedef itk::Image< PixelType, 3 >  FixedImageType;
    typedef itk::Image< PixelType, 3 >  MovingImageType;


    //unfortunately diffeomorphic demons only work with double or float types...
    // so we need to use a cast filter.
    typedef itk::Image< float, 3 > RegImageType;
    typedef float TransformScalarType;
    typedef rpi::DiffeomorphicDemons< RegImageType, RegImageType,
                    TransformScalarType > RegistrationType;
    RegistrationType * registration = new RegistrationType ();

    registrationMethod = registration;

    //convert image type so that we can still register...

    typedef itk::CastImageFilter< FixedImageType, RegImageType > CastFilterType;
    typename CastFilterType::Pointer  caster =  CastFilterType::New();
    caster->SetInput((const FixedImageType*)proc->fixedImage().GetPointer());

    registration->SetFixedImage(caster->GetOutput());
    typedef itk::CastImageFilter< MovingImageType, RegImageType > CastFilterMovingType;
    typename CastFilterType::Pointer  casterMov =  CastFilterType::New();
    casterMov->SetInput((const MovingImageType*)proc->movingImage().GetPointer());

    registration->SetMovingImage(casterMov->GetOutput());

    registration->SetNumberOfIterations(iterations);
    registration->SetMaximumUpdateStepLength(maximumUpdateStepLength);
    registration->SetUpdateFieldStandardDeviation(updateFieldStandardDeviation);
    registration->SetDisplacementFieldStandardDeviation(displacementFieldStandardDeviation);
    registration->SetUseHistogramMatching(useHistogramMatching);


    // Set update rule
    switch( updateRule )
    {
    case 0:
        registration->SetUpdateRule( RegistrationType::UPDATE_DIFFEOMORPHIC ); break;
    case 1:
        registration->SetUpdateRule( RegistrationType::UPDATE_ADDITIVE );      break;
    case 2:
        registration->SetUpdateRule( RegistrationType::UPDATE_COMPOSITIVE );   break;
    default:
        throw std::runtime_error( "Update rule must fit in the range [0,2]." );
    }


    // Set gradient type
    switch( gradientType )
    {
    case 0:
        registration->SetGradientType( RegistrationType::GRADIENT_SYMMETRIZED );
        break;
    case 1:
        registration->SetGradientType( RegistrationType::GRADIENT_FIXED_IMAGE );
        break;
    case 2:
        registration->SetGradientType( RegistrationType::GRADIENT_WARPED_MOVING_IMAGE );
        break;
    case 3:
        registration->SetGradientType( RegistrationType::GRADIENT_MAPPED_MOVING_IMAGE );
        break;
    default:
        throw std::runtime_error( "Gradient type must fit in the range [0,3]." );
    }

    // Print method parameters
    qDebug() << "METHOD PARAMETERS";

    qDebug() << "  Max number of iterations   : " << QString::fromStdString(rpi::VectorToString(registration->GetNumberOfIterations()));

    qDebug() << "  Update rule                           : " << registration->GetUpdateRule();
    qDebug() << "  Maximum step length                   : " << registration->GetMaximumUpdateStepLength()<< " (voxel unit)";
    qDebug() << "  Gradient type                         : " << registration->GetGradientType();
    qDebug() << "  Update field standard deviation       : " << registration->GetUpdateFieldStandardDeviation()         << " (voxel unit)";
    qDebug() << "  Displacement field standard deviation : " << registration->GetDisplacementFieldStandardDeviation()   << " (voxel unit)";
    qDebug() << "  Use histogram matching?               : " << registration->GetUseHistogramMatching();



    // Run the registration
    time_t t1 = clock();
    try {
        registration->StartRegistration();
    }
    catch( std::exception & err )
    {
        qDebug() << "ExceptionObject caught ! (startRegistration)" << err.what();
        return 1;
    }

    time_t t2 = clock();

    qDebug() << "Elasped time: " << (double)(t2-t1)/(double)CLOCKS_PER_SEC;

    typedef itk::ResampleImageFilter< MovingImageType,RegImageType,TransformScalarType >    ResampleFilterType;
    typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetTransform(registration->GetTransformation());
    resampler->SetInput((const MovingImageType*)proc->movingImage().GetPointer());
    //typename FixedImageType::Pointer fixedImage = fixedImage;
    resampler->SetSize( proc->fixedImage()->GetLargestPossibleRegion().GetSize() );
    resampler->SetOutputOrigin( proc->fixedImage()->GetOrigin() );
    resampler->SetOutputSpacing( proc->fixedImage()->GetSpacing() );
    resampler->SetOutputDirection( proc->fixedImage()->GetDirection() );
    resampler->SetDefaultPixelValue( 0 );

    try {
        resampler->Update();
    }
    catch (itk::ExceptionObject &e) {
        qDebug() << e.GetDescription();
        return 1;
    }

    itk::ImageBase<3>::Pointer result = resampler->GetOutput();
    result->DisconnectPipeline();


    if (proc->output())
      proc->output()->setData (result);
    return 0;
}

int itkProcessRegistrationDiffeoMorphicDemons::update(itkProcessRegistration::ImageType imgType)
{
    if(fixedImage().IsNull() || movingImage().IsNull())
        return 1;
    switch (imgType){
    //unfortunately diffeomorphic demons only work on float or double pixels...

    case itkProcessRegistration::UCHAR:
        return d->update<unsigned char>();
        break;
    case itkProcessRegistration::CHAR:
        return d->update<char>();
        break;
    case itkProcessRegistration::USHORT:
        return d->update<unsigned short>();
        break;
    case itkProcessRegistration::SHORT:
        return d->update<short>();
        break;
    case itkProcessRegistration::UINT:
       return d->update<unsigned int>();
       break;
    case itkProcessRegistration::INT:
        return d->update<int>();
        break;
    case itkProcessRegistration::ULONG:
       return d->update<unsigned long>();
       break;
    case itkProcessRegistration::LONG:
        return d->update<long>();
        break;
    case itkProcessRegistration::DOUBLE:
        return d->update<double>();
        break;
    default:
        return d->update<float>();
        break;
    }
}



template <typename FixedPixelType,typename MovingPixelType>
bool itkProcessRegistrationDiffeoMorphicDemonsPrivate::write(const QString&)
{
//    typename rpi::DiffeoMorphicDemons<FixedImageType,MovingImageType> * registration =
//            (typename rpi::DiffeoMorphicDemons<FixedImageType,MovingImageType> *)registrationMethod;

    return true;
}



bool itkProcessRegistrationDiffeoMorphicDemons::writeTransform(const QString& file)
{
    typedef float PixelType;
    typedef float TransformScalarType;
    typedef itk::Image< PixelType, 3 > RegImageType;
    //normaly should use long switch cases, but here we know we work with float3 data.
    typename rpi::DiffeomorphicDemons<RegImageType,RegImageType,TransformScalarType> * registration =
            (typename rpi::DiffeomorphicDemons<RegImageType,RegImageType,TransformScalarType> *)d->registrationMethod;
    try{

        rpi::writeDisplacementFieldTransformation<TransformScalarType, RegImageType::ImageDimension>(
                    registration->GetTransformation(),
                    file.toStdString());
    }
    catch (std::exception)
    {
        return false;
    }

    return true;
}

//bool itkProcessRegistrationDiffeoMorphicDemons::writeTransform(const QString& file)
//{
//    return false;
//}

void itkProcessRegistrationDiffeoMorphicDemons::setNumberOfIterations(std::vector<unsigned int> iterations)
{
    d->iterations = iterations;
}



// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess *createitkProcessRegistrationDiffeoMorphicDemons(void)
{
    return new itkProcessRegistrationDiffeoMorphicDemons;
}

void itkProcessRegistrationDiffeoMorphicDemons::setUpdateRule(unsigned char updateRule)
{
    d->updateRule = updateRule;
}

void itkProcessRegistrationDiffeoMorphicDemons::setGradientType(unsigned char gradientType)
{
    d->gradientType = gradientType;
}

void itkProcessRegistrationDiffeoMorphicDemons::setMaximumUpdateLength(float maximumUpdateStepLength)
{
    d->maximumUpdateStepLength = maximumUpdateStepLength;
}

void itkProcessRegistrationDiffeoMorphicDemons::setUpdateFieldStandardDeviation(float updateFieldStandardDeviation)
{
    d->updateFieldStandardDeviation = updateFieldStandardDeviation;
}

void itkProcessRegistrationDiffeoMorphicDemons::setDisplacementFieldStandardDeviation(float displacementFieldStandardDeviation)
{
    d->displacementFieldStandardDeviation = displacementFieldStandardDeviation;
}

void itkProcessRegistrationDiffeoMorphicDemons::setUseHistogramMatching(bool useHistogramMatching)
{
    d->useHistogramMatching = useHistogramMatching;
}
