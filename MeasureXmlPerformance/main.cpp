#include "main.hpp"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
XERCES_CPP_NAMESPACE_USE

#include <xqilla/utils/XQillaPlatformUtils.hpp>

#include <iostream>
#include <chrono>
#include <stack>

#define VAR_NAME(var) #var

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define DOC_VAR_NAME(doc, postFix) CONCAT(doc, postFix)
#define DOC_FRAGMENT_VAR_NAME(doc, postFix) CONCAT(doc, std::to_string(postFix))

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

void TestDeleteAddBetweenDocuments();
void TestCopyToANewDocument();
void TestMoveToANewDocumentFragment();
void TestCopySmallXmlElementsToANewDocument();

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
        //TestDeleteAddBetweenDocuments();
        //std::cout << std::endl;
        //TestCopyToANewDocument();
        //std::cout << std::endl;
        //TestMoveToANewDocumentFragment();
        //std::cout << std::endl;
        TestCopySmallXmlElementsToANewDocument();
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
    catch (const OutOfMemoryException& e)
    {
        char* msg = XMLString::transcode(e.getMessage());
        std::cout << msg << std::endl;
        XMLString::release(&msg);

        result = 1;
    }

    XQillaPlatformUtils::terminate();
    return result;
}

void MoveFirstChildOfRoot1ToANewDocument(DOMElement* root1, DOMElement* root2);
// Start move from Doc1 to Doc2
void MoveElementToAndBackBetweenDomDocument(DOMDocument* doc1, DOMDocument* doc2);

void TestDeleteAddBetweenDocuments()
{
    std::cout << "START: " << __FUNCTION__ << std::endl;

    DOMImplementation* domImpl = DOMImplementationRegistry::getDOMImplementation(XML_FEATURES);
    DOMDocument* domDoc = domImpl->createDocument();
    domDoc->appendChild(domDoc->createElement(u"root1"));

    DOMDocument* parsedDoc = nullptr;
    std::cout << "Parsed time: " << measure<>::execution([&parsedDoc]() {
        parsedDoc = ParseFile(fileToParse);
    }) << "ms" << std::endl;

    std::cout << "TOTAL time of the whole test: " << measure<>::execution([&domDoc, &parsedDoc]() {
        for (size_t i = 0; i < TEST_ITERATION; i++)
        {
            std::cout << "Total time of Iteration '" << i << "': "
                << measure<>::execution(
                    MoveElementToAndBackBetweenDomDocument
                    , parsedDoc
                    , domDoc
                ) << "ms" << std::endl;
        }
    }) << "ms" << std::endl;

    std::cout << "Free DOMDocument time: " << measure<>::execution([&domDoc]() {
        domDoc->release();
    }) << "ms" << std::endl;

    std::cout << "Free DOMDocument time: " << measure<>::execution([&parsedDoc]() {
        parsedDoc->release();
    }) << "ms" << std::endl;

    std::cout << "END: " << __FUNCTION__ << std::endl;
}

void MoveElementToAndBackBetweenDomDocument(DOMDocument* doc1, DOMDocument* doc2)
{
    DOMElement* root1 = doc1->getDocumentElement();
    DOMElement* root2 = doc2->getDocumentElement();

    std::cout << "Move first Xml Element of root1 to root2 - time: "
        << measure<>::execution(
            MoveFirstChildOfRoot1ToANewDocument
            , root1
            , root2
        ) << "ms" << std::endl;
    std::cout << "Move first Xml Element of root2 to root1 - time: "
        << measure<>::execution(
            MoveFirstChildOfRoot1ToANewDocument
            , root2
            , root1
        ) << "ms" << std::endl;
}

void MoveFirstChildOfRoot1ToANewDocument(DOMElement* root1, DOMElement* root2)
{
    DOMDocument* doc1 = root1->getOwnerDocument();
    DOMDocument* doc2 = root2->getOwnerDocument();

    DOMElement* firstChildOfRoot1 = root1->getFirstElementChild();

    DOMNode* importedNode = nullptr;
    std::cout << "Importing time: " << measure<>::execution([&importedNode, &doc2, &firstChildOfRoot1]() {
        importedNode = doc2->importNode(firstChildOfRoot1, true);
    }) << "ms" << std::endl;

    root2->appendChild(importedNode);

    root1->removeChild(firstChildOfRoot1);
    firstChildOfRoot1->release();
}

void TestCopyToANewDocument()
{
    std::cout << "START: " << __FUNCTION__ << std::endl;

    DOMImplementation* domImpl = DOMImplementationRegistry::getDOMImplementation(XML_FEATURES);

    size_t lastNum = 0;

    DOMDocument* parsedDoc = nullptr;
    std::cout << "Parsed time: " << measure<>::execution([&parsedDoc]() {
        parsedDoc = ParseFile(fileToParse);
    }) << "ms" << std::endl;

    DOMDocument* lastDoc = parsedDoc;
    parsedDoc = nullptr;
    DOMDocument* newDoc = nullptr;

    std::cout << "TOTAL time: " << measure<>::execution([&domImpl, &lastDoc, &newDoc]() {
        for (size_t i = 0; i < TEST_ITERATION; i++)
        {
            std::cout << "Copy To a new Document and Delete old Document (ITERATION: " << (i + 1) << "): " << measure<>::execution([&domImpl, &lastDoc, &newDoc]() {
                newDoc = domImpl->createDocument();
                auto importedNode = newDoc->importNode(lastDoc->getDocumentElement(), true);
                newDoc->appendChild(importedNode);

                lastDoc->release();
                lastDoc = newDoc;

                newDoc = nullptr;
            }) << "ms" << std::endl;
        }
    }) << "ms" << std::endl;

    lastDoc->release();

    std::cout << "END: " << __FUNCTION__ << std::endl;
}

void ImportOtherDocElementIntoDocFragment(DOMDocument* otherDoc, DOMDocumentFragment* fragment)
{
    DOMDocument* destinationDocument = fragment->getOwnerDocument();
    DOMNode* rootNode = fragment->getFirstChild();
    if (rootNode == nullptr)
        rootNode = fragment->appendChild(destinationDocument->createElement(u"root"));

    auto otherDocChildNode = otherDoc->getFirstChild();
    while (otherDocChildNode != nullptr)
    {
        auto importedNode = destinationDocument->importNode(otherDocChildNode, true);
        rootNode->appendChild(importedNode);
        otherDocChildNode = otherDocChildNode->getNextSibling();
    }
}

void TestMoveToANewDocumentFragment()
{
    std::cout << "START: " << __FUNCTION__ << std::endl;

    DOMImplementation* domImpl = DOMImplementationRegistry::getDOMImplementation(XML_FEATURES);
    DOMDocument* domDocument = domImpl->createDocument();

    DOMDocument* parsedDoc = nullptr;

    DOMDocumentFragment* lastDocFragment = domDocument->createDocumentFragment();
    lastDocFragment->appendChild(domDocument->createElement(u"root"));

    DOMDocumentFragment* newDocFragment = nullptr;

    std::cout << "Parsed time: " << measure<>::execution([&parsedDoc, &lastDocFragment]() {
        parsedDoc = ParseFile(fileToParse);
        ImportOtherDocElementIntoDocFragment(parsedDoc, lastDocFragment);
        parsedDoc->release();
    }) << "ms" << std::endl;

    std::cout << "TOTAL time: " << measure<>::execution([&lastDocFragment, &newDocFragment]() {
        for (size_t i = 0; i < TEST_ITERATION; i++)
        {
            std::cout << "Copy To a new DocFragment and Release old DocFragment (ITERATION: " << (i + 1) << "): " << measure<>::execution([&lastDocFragment, &newDocFragment]() {
                DOMDocument* domDoc = lastDocFragment->getOwnerDocument();
                newDocFragment = domDoc->createDocumentFragment();
                DOMElement* newRoot = domDoc->createElement(u"root");
                newDocFragment->appendChild(newRoot);

                DOMElement* lastRoot = dynamic_cast<DOMElement*>(lastDocFragment->getFirstChild());
                DOMNode* elementToMove = lastRoot->removeChild(lastRoot->getFirstChild());

                newRoot->appendChild(dynamic_cast<DOMElement*>(elementToMove));

                lastDocFragment->release();
                lastDocFragment = newDocFragment;

                newDocFragment = nullptr;
            }) << "ms" << std::endl;
        }
    }) << "ms" << std::endl;

    domDocument->release();

    std::cout << "END: " << __FUNCTION__ << std::endl;
}

void TestCopySmallXmlElementsToANewDocument()
{
    std::cout << "START: " << __FUNCTION__ << std::endl;

    DOMImplementation* domImpl = DOMImplementationRegistry::getDOMImplementation(XML_FEATURES);

    DOMDocument* bigXmlDoc = nullptr;
    std::cout << "Parsed time: " << measure<>::execution([&bigXmlDoc]() {
        bigXmlDoc = ParseFile("VeryBigXmlFile.xml");
    }) << "ms" << std::endl;

    DOMDocument* smallXmlDoc1 = nullptr;
    std::cout << "Parsed time: " << measure<>::execution([&smallXmlDoc1]() {
        smallXmlDoc1 = ParseFile("simple.xml");
    }) << "ms" << std::endl;

    std::cout << "TOTAL time: " << measure<>::execution([&bigXmlDoc, &smallXmlDoc1]() {
        DOMElement* childElement = smallXmlDoc1->getDocumentElement()->getFirstElementChild();

        while (childElement != nullptr)
        {
            std::cout << "Add small Xml Element to a big Document - elapsed time: " << measure<>::execution([&bigXmlDoc, &childElement]() {
                auto importedNode = bigXmlDoc->importNode(childElement, true);
                bigXmlDoc->getDocumentElement()->appendChild(importedNode);
            }) << "ms" << std::endl;

            childElement = childElement->getNextElementSibling();
        }
    }) << "ms" << std::endl;

    bigXmlDoc->release();
    smallXmlDoc1->release();

    std::cout << "END: " << __FUNCTION__ << std::endl;
}
