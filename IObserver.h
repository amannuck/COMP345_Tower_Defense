/**
 * @file IObserver.h
 * @author amann
 * @date 27/02/2025
 * @brief Observer pattern interfaces for event handling
 * @details Implements the Observer pattern with IObserver and IObservable interfaces.
 *          These interfaces enable objects to subscribe to and be notified of changes
 *          in other objects without tight coupling.
 */

#ifndef IOBSERVER_H
#define IOBSERVER_H

/**
 * @class IObserver
 * @brief Interface for observer objects
 * @details Classes implementing this interface can receive notifications
 *          about changes in observable objects they're registered with.
 */
class IObserver {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~IObserver() = default;
    
    /**
     * @brief Update method called when observable state changes
     * @details This method will be called by the observable object when
     *          its state changes. Derived classes should implement this
     *          method to respond to notifications appropriately.
     */
    virtual void update() = 0;
};

/**
 * @class IObservable
 * @brief Interface for observable objects
 * @details Classes implementing this interface can maintain a list of observers
 *          and notify them of state changes. This allows for loose coupling
 *          between objects that interact with each other.
 */
class IObservable {
private:
    std::vector<IObserver*> observers; ///< Collection of registered observers

public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~IObservable() = default;
    
    /**
     * @brief Registers an observer to receive notifications
     * @param observer Pointer to the observer object
     * @details Adds the observer to the internal collection of observers.
     *          The observer will be notified when notifyObservers() is called.
     */
    void addObserver(IObserver* observer) {
        observers.push_back(observer);
    }
    
    /**
     * @brief Unregisters an observer from receiving notifications
     * @param observer Pointer to the observer object to remove
     * @details Removes the observer from the internal collection using
     *          the erase-remove idiom. After removal, the observer will
     *          no longer receive notifications.
     */
    void removeObserver(IObserver* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    /**
     * @brief Notifies all registered observers of a state change
     * @details Calls the update() method on each registered observer.
     *          This method should be called whenever the observable's
     *          state changes in a way that observers should be aware of.
     */
    void notifyObservers() {
        for (auto observer : observers) {
            observer->update();
        }
    }
};

#endif //IOBSERVER_H
