#pragma once
#include <memory>


/*шаблонный класс-компонент, реализующий управление сессией.  онкретные действи€ завис€т от того, какой класс был выбран в качестве Handler*/
/* ¬от только тогда нужно ограничивать доступные в шаблоне классы, чтобы не произошло непри€тных ситуаций*/
template<typename Handler>
class SessionComponent {
public:
	SessionComponent(Handler& handler) : sessionHandler(std::make_unique<Handler>(handler)) {}
	virtual ~SessionComponent() = default;
private:
	std::unique_ptr<Handler> sessionHandler;
};
