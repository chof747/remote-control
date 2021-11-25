/** Check if the message topic matches the given subscription topic
 * 
 *  Source: Code taken from MQTT component of (mqtt_client.cpp commit e6f8e73)
 *          the original code can be found here: 
 *           https://github.com/esphome/esphome/blob/dev/esphome/components/mqtt/mqtt_client.cpp
 *
 * INFO: MQTT spec mandates that topics must not be empty and must be valid NULL-terminated UTF-8 strings.
 *
 * @param message The message topic that was received from the MQTT server. Note: this must not contain
 *                wildcard characters as mandated by the MQTT spec.
 * @param subscription The subscription topic we are matching against.
 * @param is_normal Is this a "normal" topic - Does the message topic not begin with a "$".
 * @param past_separator Are we past the first '/' topic separator.
 * @return true if the subscription topic matches the message topic, false otherwise.
 */
static bool topic_match(const char *message, const char *subscription, bool is_normal, bool past_separator) 
//****************************************************************************************
{
  // Reached end of both strings at the same time, this means we have a successful match
  if (*message == '\0' && *subscription == '\0')
    return true;

  // Either the message or the subscribe are at the end. This means they don't match.
  if (*message == '\0' || *subscription == '\0')
    return false;

  bool do_wildcards = is_normal || past_separator;

  if (*subscription == '+' && do_wildcards) 
  {
    // single level wildcard
    // consume + from subscription
    subscription++;
    // consume everything from message until '/' found or end of string
    while (*message != '\0' && *message != '/') 
    {
      message++;
    }
    // after this, both pointers will point to a '/' or to the end of the string

    return topic_match(message, subscription, is_normal, true);
  }

  if (*subscription == '#' && do_wildcards) {
    // multilevel wildcard - MQTT mandates that this must be at end of subscribe topic
    return true;
  }

  // this handles '/' and normal characters at the same time.
  if (*message != *subscription)
    return false;

  past_separator = past_separator || *subscription == '/';

  // consume characters
  subscription++;
  message++;

  return topic_match(message, subscription, is_normal, past_separator);
}

static bool topic_match(const char *message, const char *subscription) {
  return topic_match(message, subscription, *message != '\0' && *message != '$', false);
}
