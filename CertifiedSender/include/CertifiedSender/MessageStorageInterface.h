/*
 * MessageStorageInterface.h
 *
 * Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef ALEXA_CLIENT_SDK_CERTIFIED_SENDER_INCLUDE_CERTIFIED_SENDER_MESSAGE_STORAGE_INTERFACE_H_
#define ALEXA_CLIENT_SDK_CERTIFIED_SENDER_INCLUDE_CERTIFIED_SENDER_MESSAGE_STORAGE_INTERFACE_H_

#include <memory>
#include <string>
#include <queue>

namespace alexaClientSDK {
namespace certifiedSender {

/**
 * An Interface class which defines APIs for interacting with a database for storing text-based messages.
 *
 * An implementation of this interface must enforce ordering of the messages, so that the ordering of items returned
 * in the load() operation are the same as the ordering of store() calls.
 *
 * This interface does not provide any thread-safety guarantees.
 */
class MessageStorageInterface {
public:
    /**
     * Utility structure to express a message stored in a database.
     */
    struct StoredMessage {
        /**
         * Default Constructor.
         */
        StoredMessage() : id{0} {
        }

        /**
         * Constructor.
         *
         * @param id The id which the database implementation associates with the message.
         * @param message The text message which has been stored in the database.
         */
        StoredMessage(int id, const std::string& message) : id{id}, message{message} {
        }

        /// The unique id which the database associates with this message.
        int id;
        /// The message being stored.
        std::string message;
    };

    /**
     * Destructor.
     */
    virtual ~MessageStorageInterface() = default;

    /**
     * Creates a new database with the given filePath.
     * If the file specified already exists, or if a database is already being handled by this object, then
     * this function returns false.
     *
     * @param filePath The path to the file which will be used to contain the database.
     * @return @c true If the database is created ok, or @c false if either the file exists or a database is already
     * being handled by this object.
     */
    virtual bool createDatabase(const std::string& filePath) = 0;

    /**
     * Open a database with the given filePath.  If this object is already managing an open database, or the file
     * does not exist, or there is a problem opening the database, this function returns false.
     *
     * @param filePath The path to the file which will be used to contain the database.
     * @return @c true If the database is opened ok, @c false if either the file does not exist, if this object is
     * already managing an open database, or if there is another internal reason the database could not be opened.
     */
    virtual bool open(const std::string& filePath) = 0;

    /**
     * Query if this object is currently managing an open database.
     *
     * @return @c true If a database is being currently managed by this object, @c false otherwise.
     */
    virtual bool isOpen() = 0;

    /**
     * Close the currently open database, if one is open.
     */
    virtual void close() = 0;

    /**
     * Stores a single message in the database.
     *
     * @param The message to store.
     * @param[out] id The id associated with the stored messsage, if successfully stored.
     * @return Whether the message was successfully stored.
     */
    virtual bool store(const std::string& message, int* id) = 0;

    /**
     * Loads all messages in the database.
     *
     * @param[out] messageContainer The container where messages should be stored.
     * @return Whether the @c StoredMessages were successfully loaded.
     */
    virtual bool load(std::queue<StoredMessage>* messageContainer) = 0;

    /**
     * Erases a single message from the database.
     *
     * @param messageId The id of the message to be erased.
     * @return Whether the message was successfully erased.
     */
    virtual bool erase(int messageId) = 0;

    /**
     * A utility function to clear the database of all records.  Note that the database will still exist, as will
     * the tables.  Only the rows will be erased.
     *
     * @return Whether the database was successfully cleared.
     */
    virtual bool clearDatabase() = 0;
};

}  // namespace certifiedSender
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_CERTIFIED_SENDER_INCLUDE_CERTIFIED_SENDER_MESSAGE_STORAGE_INTERFACE_H_
