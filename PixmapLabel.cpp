#include "PixmapLabel.h"

PixmapLabel::PixmapLabel(QWidget* parent) noexcept : QLabel(parent)
{
}

void PixmapLabel::resizeEvent(QResizeEvent* event)  noexcept
{
	QLabel::resizeEvent(event);
	if (!m_pix.isNull()) {
		m_pix = m_pix.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		QLabel::setPixmap(m_pix);
	}
}

void PixmapLabel::setPixmap(const QPixmap& pix) noexcept
{
	m_pix = pix;
	QLabel::setPixmap(m_pix);
}
