#ifndef TESTGITSTORAGE_H
#define TESTGITSTORAGE_H

#include <QTest>

class TestGitStorage : public QObject
{
	Q_OBJECT
private slots:
	void testSetup();
	void testGitStorageLocal();
	void testGitStorageCloud();
	void testGitStorageCloudOfflineSync();
	void testGitStorageCloudMerge();
	void testGitStorageCloudMerge2();
	void testGitStorageCloudMerge3();
};

#endif // TESTGITSTORAGE_H
