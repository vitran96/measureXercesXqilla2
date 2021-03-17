#include "main.hpp"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
XERCES_CPP_NAMESPACE_USE

#include <xqilla/utils/XQillaPlatformUtils.hpp>

#include <iostream>
#include <chrono>

const XMLCh* XML_FEATURES = u"XPath2";
std::string fileToParse = "sample.xml";

const size_t TEST_ITERATION = 10;

// TODO: consider move to separate header
template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args)
    {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};

class DOMPrintErrorHandler : public DOMErrorHandler
{
public:

    DOMPrintErrorHandler() {};
    ~DOMPrintErrorHandler() {};

    /** @name The error handler interface */
    bool handleError(const DOMError& domError) override
    {
        // Display whatever error message passed from the serializer
        if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
            std::cerr << "\nWarning Message: ";
        else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
            std::cerr << "\nError Message: ";
        else
            std::cerr << "\nFatal Message: ";

        char* msg = XMLString::transcode(domError.getMessage());
        std::cerr << msg << std::endl;
        XMLString::release(&msg);

        // Instructs the serializer to continue serialization if possible.
        return true;
    }

    void resetErrors() {};

private:
    /* Unimplemented constructors and operators */
    //DOMPrintErrorHandler(const DOMErrorHandler &);
    //void operator=(const DOMErrorHandler &);

};

DOMDocument* ParseFile(const std::string& file)
{
    XercesDOMParser parser;
    parser.setValidationScheme(XercesDOMParser::Val_Auto);
    parser.setDoNamespaces(true);
    parser.useImplementation(XML_FEATURES);
    parser.parse(file.c_str());

    return parser.adoptDocument();
}

int main(int argc, char** argv)
{
    int result = 0;

    if (argc > 1)
    {
        fileToParse = std::string(argv[1]);
    }

    try
    {
        XQillaPlatformUtils::initialize();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        result = 1;
    }
    catch (const DOMException& e)
    {
        char* msg = XMLString::transcode(e.getMessage());
        std::cout << msg << std::endl;
        XMLString::release(&msg);

        result = 1;
    }

    XQillaPlatformUtils::terminate();
    return result;
}
