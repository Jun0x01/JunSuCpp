
// JunSuCppDoc.h : interface of the JunSuCppDoc class
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */


#pragma once


class JunSuCppDoc : public CDocument
{
protected: // create from serialization only
	JunSuCppDoc() noexcept;
	DECLARE_DYNCREATE(JunSuCppDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~JunSuCppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
