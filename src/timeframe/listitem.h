#ifndef LISTITEM_H
#define LISTITEM_H


class ListItem
{
public:
    virtual ~ListItem() {}
    virtual QString id() const = 0;
    virtual QVariant data( int role ) const = 0;
};

#endif // LISTITEM_H
