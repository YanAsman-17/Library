#pragma once
#include"qlabel.h"
#include"qwidget.h"

class PixmapLabel : public QLabel
{
public:
	explicit PixmapLabel(QWidget* parent = nullptr) noexcept;
	void setPixmap(const QPixmap& pix) noexcept;
private:
	QPixmap m_pix;
	void resizeEvent(QResizeEvent* event) noexcept override;
};

